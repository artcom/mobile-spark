// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "ACProjectView.h"
#include <cstdlib>

#include <cml/mathlib/matrix_transform.h>
#include <masl/Logger.h>
#include <masl/MobileSDK.h>
#include <masl/numeric_functions.h>

#include <masl/AssetProvider.h>
#include <mar/Shape.h>
#include <mar/Material.h>

#include <spark/Window.h>
#include <spark/Event.h>
#include <spark/SparkComponentFactory.h>
#include <spark/AppProvider.h>


#include <animation/AnimationManager.h>
#include <animation/ParallelAnimation.h>
#include <animation/PropertyAnimation.h>
#include <animation/Easing.h>

#include "ACProjectViewComponentMapInitializer.h"
#include "AppLoaderAnim.h"

using namespace spark;
using namespace masl;
using namespace mar;

/////////////////////////////////////////////////////////////////////////App-Instance
#ifdef ANDROID
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    spark::AppProvider::get().setApp(masl::Ptr<acprojectview::ACProjectView>(new acprojectview::ACProjectView()));
    return JNI_VERSION_1_6;
}
#endif


/////////////////// Application code, this should be in java or script language later...
namespace acprojectview {
   
    const std::string ACProjectView::BASE_URL = "http://www.einsfeld.de/mobile-spark/ACProjectView/";

    ACProjectView::ACProjectView():BaseApp("ACProjectView"), 
        firstIdleImageVisible_(true), swappedIdleImages_(true), _myAnimatingFlag(false),
        loadCount_(0), isRealized_(false), _myOnlineMode(false) {
    } 

    ACProjectView::~ACProjectView() {
    }

    void ACProjectView::setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, int theScreenWidth, int theScreenHeight) {
        BaseApp::setup(theCurrentMillis, theAssetPath, theScreenWidth, theScreenHeight);

        ACProjectViewComponentMapInitializer::init();        
        ACProjectViewPtr ptr = boost::static_pointer_cast<ACProjectView>(shared_from_this());
        loadLayoutAndRegisterEvents(theScreenWidth, theScreenHeight);

        spark::TextPtr myGermanButton = boost::static_pointer_cast<Text>(_mySparkWindow->getChildByName("deutsch", true));
        spark::TextPtr myEnglishButton = boost::static_pointer_cast<Text>(_mySparkWindow->getChildByName("english", true));
        spark::RectanglePtr myRightBar = boost::static_pointer_cast<Rectangle>(_mySparkWindow->getChildByName("right_bar", true));
        spark::RectanglePtr myLangSep = boost::static_pointer_cast<Rectangle>(_mySparkWindow->getChildByName("lang_sep", true));
        myLangSep->setSize(vector2(myLangSep->getShape()->getBoundingBox().max[0], myGermanButton->getTextSize()[1]));

        myEnglishButton->setX(_mySparkWindow->getSize()[0] - myEnglishButton->getTextSize()[0] - 12);
        myLangSep->setX( myEnglishButton->getX() - 12);
        myGermanButton->setX( myLangSep->getX() - myGermanButton->getTextSize()[0] - 12);
        myRightBar->setX(_mySparkWindow->getSize()[0] - myRightBar->getNode()->getAttributeAs<float>("width",0));
        
        spark::EventCallbackPtr mySwitchLanguageDeCB = EventCallbackPtr(new ACProjectViewEventCB(ptr, &ACProjectView::onLanguageSwitchDe));
        myGermanButton->addEventListener(TouchEvent::PICKED, mySwitchLanguageDeCB);

        spark::EventCallbackPtr mySwitchLanguageEnCB = EventCallbackPtr(new ACProjectViewEventCB(ptr, &ACProjectView::onLanguageSwitchEn));
        myEnglishButton->addEventListener(TouchEvent::PICKED, mySwitchLanguageEnCB);
            
        spark::EventCallbackPtr myWorldRealizedCB = EventCallbackPtr(new ACProjectViewEventCB(ptr, &ACProjectView::onWorldRealized));
        _mySparkWindow->addEventListener(WindowEvent::WORLD_REALIZED, myWorldRealizedCB);
        spark::WidgetPtr myLoadAnim = boost::static_pointer_cast<Window>(_mySparkWindow->getChildByName("loaderworld")->getChildByName("apploaderanim", true));
        myLoadAnim->setX(_mySparkWindow->getSize()[0]/2.0);
        myLoadAnim->setY(_mySparkWindow->getSize()[1]/2.0);

        //start to load content from server
        if (_myOnlineMode) {
            std::vector<std::string> myOnSetupNeededAssets;
            myOnSetupNeededAssets.push_back("i18n.spark");
            myOnSetupNeededAssets.push_back("inner_menu.spark");
            _myRequestManager.setOnErrorCallback(
                masl::RequestCallbackPtr(new ACProjectViewRequestCB(ptr, &ACProjectView::onErrorRequest)));
            _myRequestManager.getAllRequest(ACProjectView::BASE_URL + "/layouts/", myOnSetupNeededAssets, 
                masl::RequestCallbackPtr(new ACProjectViewRequestCB(ptr, &ACProjectView::onAssetReady)),
                masl::RequestCallbackPtr(), "/downloads/", true, masl::REQUEST_IF_NEWER);
        } else {
            loadOfflineVersion();                   
        }
    }
    
    void ACProjectView::loadOfflineVersion() {
        ACProjectViewPtr ptr = boost::static_pointer_cast<ACProjectView>(shared_from_this());
        
        // load global i18n 
        std::string myNewSpark = "i18n.spark";
        ComponentPtr myNewSparkComponent = spark::SparkComponentFactory::get().loadSparkComponentsFromFile(ptr, myNewSpark);
        spark::TransformPtr myTransform = boost::static_pointer_cast<spark::Transform>(_mySparkWindow->getChildByName("global-i18n"));
        spark::ContainerPtr myNewSparkContainer = boost::static_pointer_cast<spark::Container>(myNewSparkComponent);
        for (VectorOfComponentPtr::const_iterator it = myNewSparkContainer->getChildren().begin(); it != myNewSparkContainer->getChildren().end(); ++it) {
            myTransform->addChild(*it);
        }
        // load global widgets
        myTransform = boost::static_pointer_cast<spark::Transform>(_mySparkWindow->getChildByName("2dworld"));
        myNewSparkComponent = spark::SparkComponentFactory::get().loadSparkComponentsFromFile(ptr, "inner_menu.spark", myTransform); 
        std::string myInnerMenu = masl::AssetProviderSingleton::get().ap()->getStringFromFile("inner_menu.spark");
        idleFiles_ = spark::SparkComponentFactory::get().createSrcListFromSpark(myInnerMenu);
        AC_DEBUG << ".................... idle files size " << idleFiles_.size();
        //for (std::vector<std::string>::iterator it = idleFiles_.begin(); it != idleFiles_.end(); ++it) {
        //    AC_PRINT << *it;
        //}
        onLoadComplete();
        loadCount_ = 3;        
    }
    
    void ACProjectView::onErrorRequest(masl::RequestPtr theRequest) {
        AC_ERROR << "............ACProjectView onError for URL " << theRequest->getURL();
        //display error message if data has not been persisted
        /*if (masl::AssetProviderSingleton::get().ap()->findFile("/downloads/i18n.spark").empty()) {
            TextPtr myErrorText = boost::static_pointer_cast<Text>(_mySparkWindow->getChildByName("loaderworld")->getChildByName("error"));
            myErrorText->setVisible(true);
            myErrorText->setText("network error: internet connection required for initial setup");
        }*/
        loadOfflineVersion();
    }

    void ACProjectView::onAssetReady(masl::RequestPtr theRequest) {
        loadCount_++;
        ACProjectViewPtr ptr = boost::static_pointer_cast<ACProjectView>(shared_from_this());
        if (loadCount_ == 1) {
            AC_DEBUG << ".............................onI18nRequestReady";
            std::string myNewSpark = theRequest->getResponseString();
            ComponentPtr myNewSparkComponent = spark::SparkComponentFactory::get().loadSparkComponentsFromString(ptr, myNewSpark);
            spark::TransformPtr myTransform = boost::static_pointer_cast<spark::Transform>(_mySparkWindow->getChildByName("global-i18n"));
            spark::ContainerPtr myNewSparkContainer = boost::static_pointer_cast<spark::Container>(myNewSparkComponent);
            for (VectorOfComponentPtr::const_iterator it = myNewSparkContainer->getChildren().begin(); it != myNewSparkContainer->getChildren().end(); ++it) {
                myTransform->addChild(*it);
            }
        } else if (loadCount_ == 2) {
            AC_DEBUG << ".............................onMenuRequestReady " << theRequest;
            std::string myNewSpark = theRequest->getResponseString();
            std::vector<std::string> assetList = spark::SparkComponentFactory::get().createSrcListFromSpark(myNewSpark);
            idleFiles_ = assetList;
            std::vector<std::string> assetFilteredList;
            std::string dummyString;
            for (size_t i=0; i<assetList.size();++i) {
                if (!searchFile(assetList[i], dummyString, false)) {
                    assetFilteredList.push_back(assetList[i]);
                } else {
                    AC_DEBUG << " found Asset: " << assetList[i];  
                }
            }
            AC_DEBUG << "...........................num idle files online version " << idleFiles_.size();
            _myRequestManager.getAllRequest(BASE_URL+"/textures/", assetFilteredList,
                masl::RequestCallbackPtr(),
                masl::RequestCallbackPtr(new ACProjectViewRequestCB(ptr, &ACProjectView::onAssetReady)),
                "/downloads/", true, masl::REQUEST_IF_NOT_AVAILABLE);
        } else if (loadCount_ == 3) {
            AC_DEBUG << ".............................onAssetsReady";
            spark::TransformPtr myTransform = boost::static_pointer_cast<spark::Transform>(_mySparkWindow->getChildByName("2dworld"));
            ComponentPtr myNewSparkComponent = spark::SparkComponentFactory::get().loadSparkComponentsFromFile(ptr, "inner_menu.spark", myTransform);
            onLoadComplete();
        }
    }

    void ACProjectView::onLoadComplete() {
        AC_DEBUG << "onLoadComplete";
        ACProjectViewPtr ptr = boost::static_pointer_cast<ACProjectView>(shared_from_this());
        _myProjectMenu =  boost::static_pointer_cast<ProjectMenu>(_mySparkWindow->getChildByName("2dworld")->getChildByName("main",true));
        _myProjectViewer = boost::static_pointer_cast<ProjectViewerImpl>(_mySparkWindow->getChildByName("2dworld")->getChildByName("projectViewer",true));
        spark::RectanglePtr myBackground = boost::static_pointer_cast<Rectangle>(_mySparkWindow->getChildByName("2dworld")->getChildByName("background2"));
        myBackground->setSize(_mySparkWindow->getSize());
                    
        _myProjectMenu->setSensible(false);
        _myStartScreenPtr =  boost::static_pointer_cast<Transform>(_mySparkWindow->getChildByName("2dworld")->getChildByName("startscreen",true));
        spark::RectanglePtr myStartBackground = boost::static_pointer_cast<Rectangle>(_myStartScreenPtr->getChildByName("background"));
        myStartBackground->setSize(_mySparkWindow->getSize());
               
        spark::EventCallbackPtr myPickedCB = EventCallbackPtr(new ACProjectViewEventCB(ptr, &ACProjectView::onProjectItem));
        spark::EventCallbackPtr myBackCB = EventCallbackPtr(new ACProjectViewEventCB(ptr, &ACProjectView::onBack));
        
        _myProjectViewer->addEventListener(TouchEvent::PICKED, myBackCB,Event::BUBBLING);
        
        _myProjectItems = boost::static_pointer_cast<spark::Container>(_myProjectMenu);
        const VectorOfComponentPtr & myChildren = _myProjectItems->getChildrenByType(ProjectImpl::SPARK_TYPE);
        for (size_t i = 0; i < myChildren.size(); i++) {
            ProjectImplPtr myProject = boost::static_pointer_cast<ProjectImpl>(myChildren[i]);
            if (myProject) {
                myProject->addEventListener(TouchEvent::PICKED, myPickedCB,Event::CAPTURING);
            }
        }

        if (isRealized_) {
            onAllReady();
        }
        
        
    }
    void ACProjectView::onFrame(EventPtr theEvent) {
        BaseApp::onFrame(theEvent);
        _myRequestManager.handleRequests(); //TODO: move to BaseApp?
    }
    
    void ACProjectView::onResume() {
        BaseApp::onResume();
        fitToWindowSize(_myIdleScreenImagePtrs[0]);
        fitToWindowSize(_myIdleScreenImagePtrs[1]);
    }
   
    void ACProjectView::onStartIdleFade() {
        _myStartScreenPtr->setVisible(true);
    }

    void ACProjectView::onFinishIdleFade() {
        _myStartScreenPtr->setVisible(false);
    }

    void ACProjectView::onProjectItem(EventPtr theEvent) {
        if (_myAnimatingFlag) {
            if (_mySeqAnimation) {
                _mySeqAnimation->cancel();
            }
            //return;
        }
        MobileSDK_Singleton::get().getNative()->vibrate(10);                
        _myProjectViewer->setVisible(true);
        _myProjectViewer->setSensible(true);
        _myProjectMenu->setSensible(false);
        _myCurrentProject = boost::static_pointer_cast<ProjectImpl>(theEvent->getTarget()->getParent().lock());
        projectViewAnimation(true);
    }

    
    void ACProjectView::onBack(EventPtr theEvent) {
        if (_myAnimatingFlag) {
            if (_mySeqAnimation) {
                _mySeqAnimation->cancel();
            }
        }
        TouchEventPtr myEvent = boost::static_pointer_cast<TouchEvent>(theEvent);
        if (myEvent->getY() > ProjectViewerImpl::POPUP_HEIGHT * 2 && !_myProjectViewer->isPopUpOpen()) { 
            MobileSDK_Singleton::get().getNative()->vibrate(10);                
            projectViewAnimation(false);
            _myProjectViewer->setSensible(false);
        }
    }
    
    void ACProjectView::onInitiateProjectView() {   
        _myProjectViewer->showProject(_myCurrentProject);        
    }

    void ACProjectView::onStartProjectView() {        
        _myProjectViewer->setVisible(true);
    }    
    void ACProjectView::onShowProjectViewPopup() {        
        _myProjectViewer->showPopup(true);
    }    
    
    void ACProjectView::onFinishLoadProjectView() {  
        _myProjectMenu->setVisible(false);
        _myProjectMenu->setSensible(false);        
        _myAnimatingFlag = false;        
    }

    void ACProjectView::closeProjectView() {        
        _myProjectViewer->setVisible(false);
        _myProjectMenu->setVisible(true);
        _myProjectMenu->setSensible(true);                
        _myAnimatingFlag = false;        
    }
    void ACProjectView::onReturn2ProjectView() {        
        _myProjectViewer->initiateClose();        
        _myProjectViewer->showPopup(false);
        _myProjectMenu->setVisible(true);
    }
    
    void ACProjectView::onLoadInitialSet0() {        
        _myProjectViewer->loadInitialSet0();
    }
    void ACProjectView::onLoadInitialSet1() {        
        _myProjectViewer->loadInitialSet1();
    }
    void ACProjectView::onLoadInitialSet2() {        
        _myProjectViewer->loadInitialSet2();
    }
    void ACProjectView::onLoadInitialSet3() {        
        _myProjectViewer->loadInitialSet3();
    }
    void ACProjectView::onLoadInitialSet4() {        
        _myProjectViewer->loadInitialSet4();
    }
    
    
    void ACProjectView::projectViewAnimation(bool showProject){
        int myProjectMenuItemWidth = _myProjectMenu->getIconWidth();
        int myProjectMenuItemHeight = _myProjectMenu->getIconHeight();
        int mySlide = _myProjectMenu->getCurrentSlide();
        _myAnimatingFlag = true;
        _mySeqAnimation = animation::SequenceAnimationPtr(new animation::SequenceAnimation());
        ACProjectViewPtr ptr = boost::static_pointer_cast<ACProjectView>(shared_from_this());
        int toX = showProject ? 0 : _myCurrentProject->getX()+ myProjectMenuItemWidth/2 - mySlide*_mySparkWindow->getSize()[0];
        int fromX   = showProject ? _myCurrentProject->getX()+ myProjectMenuItemWidth/2  - mySlide*_mySparkWindow->getSize()[0]: _myProjectViewer->getX();
        int toY = showProject ? 0 : _myCurrentProject->getY()+ myProjectMenuItemHeight/2;
        int fromY   = showProject ? _myCurrentProject->getY()+ myProjectMenuItemHeight/2 : _myProjectViewer->getY();
        float toScale = showProject ? 1 :  0;
        float fromScale   = showProject ? 0 : _myProjectViewer->getScaleX();//showProject ? 0 :  1;
        WidgetPropertyAnimationPtr myZoomAnimationX = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(WidgetWeakPtr(WidgetPtr(_myProjectViewer)), &Widget::setScaleX, fromScale, toScale, _myAnimationTime,
                    animation::EasingFnc(animation::easeInOutQuad)));
        WidgetPropertyAnimationPtr myZoomAnimationY = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(WidgetWeakPtr(WidgetPtr(_myProjectViewer)), &Widget::setScaleY, fromScale, toScale, _myAnimationTime,
                    animation::EasingFnc(animation::easeInOutQuad)));
        WidgetPropertyAnimationPtr myTransAnimationX = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(WidgetWeakPtr(WidgetPtr(_myProjectViewer)), &Widget::setX, fromX, toX, _myAnimationTime,
                    animation::EasingFnc(animation::easeInOutQuad)));
        WidgetPropertyAnimationPtr myTransAnimationY = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(WidgetWeakPtr(WidgetPtr(_myProjectViewer)), &Widget::setY, fromY, toY, _myAnimationTime,
                    animation::EasingFnc(animation::easeInOutQuad)));
           
        animation::ParallelAnimationPtr myParallel = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
        myParallel->setOnPlay(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::onStartProjectView)));
        myParallel->setOnFinish(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::onShowProjectViewPopup)));

        myParallel->add(myZoomAnimationX);
        myParallel->add(myZoomAnimationY);
        myParallel->add(myTransAnimationX);
        myParallel->add(myTransAnimationY);
        if (showProject) {                       
            animation::DelayAnimationPtr myPreAnimation = animation::DelayAnimationPtr(new animation::DelayAnimation(2));                
            myPreAnimation->setOnPlay(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::onInitiateProjectView))); 
            _mySeqAnimation->add(myPreAnimation);
                
            _mySeqAnimation->setOnFinish(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::onFinishLoadProjectView)));
            _mySeqAnimation->setOnCancel(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::onFinishLoadProjectView)));
        }
        _mySeqAnimation->add(myParallel);
        if (showProject) {                       
            animation::DelayAnimationPtr myFrameDelayAnim1 = animation::DelayAnimationPtr(new animation::DelayAnimation(2));
            _mySeqAnimation->add(myFrameDelayAnim1);

            animation::DelayAnimationPtr myloadInitial0Anim= animation::DelayAnimationPtr(new animation::DelayAnimation(200));
            myloadInitial0Anim->setOnPlay(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::onLoadInitialSet0)));

            animation::DelayAnimationPtr myloadInitial1Anim= animation::DelayAnimationPtr(new animation::DelayAnimation(200));
            myloadInitial1Anim->setOnPlay(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::onLoadInitialSet1)));

            animation::DelayAnimationPtr myloadInitial2Anim= animation::DelayAnimationPtr(new animation::DelayAnimation(200));
            myloadInitial2Anim->setOnPlay(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::onLoadInitialSet2)));
                
            animation::DelayAnimationPtr myloadInitial3Anim = animation::DelayAnimationPtr(new animation::DelayAnimation(200));
            myloadInitial3Anim->setOnPlay(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::onLoadInitialSet3)));

            animation::DelayAnimationPtr myloadInitial4Anim = animation::DelayAnimationPtr(new animation::DelayAnimation(200));
            myloadInitial4Anim->setOnPlay(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::onLoadInitialSet4)));
                
            _mySeqAnimation->add(myloadInitial0Anim);
            _mySeqAnimation->add(myloadInitial1Anim);
            _mySeqAnimation->add(myloadInitial2Anim);
            _mySeqAnimation->add(myloadInitial3Anim);
            _mySeqAnimation->add(myloadInitial4Anim);
        } else {
            _mySeqAnimation->setOnFinish(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::closeProjectView)));
            _mySeqAnimation->setOnCancel(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::closeProjectView)));
            _mySeqAnimation->setOnPlay(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::onReturn2ProjectView)));
        }

        animation::AnimationManager::get().play(_mySeqAnimation);
    }

    void ACProjectView::onLanguageSwitchDe(EventPtr theEvent) {
        _mySparkWindow->switchLanguage(spark::DE);
    }
    void ACProjectView::onLanguageSwitchEn(EventPtr theEvent) {
        _mySparkWindow->switchLanguage(spark::EN);
    }
    
    void ACProjectView::onAllReady() {
        AC_DEBUG << "_______________________________ON ALL READY";
        boost::static_pointer_cast<View>(_mySparkWindow->getChildByName("loaderView"))->setVisible(false);
        boost::static_pointer_cast<View>(_mySparkWindow->getChildByName("mainView"))->setVisible(true);
        AppLoaderAnimPtr myLoadAnim = boost::static_pointer_cast<AppLoaderAnim>(_mySparkWindow->getChildByName("loaderworld")->getChildByName("apploaderanim", true));
        myLoadAnim->removeOnFrameListener();
        initIdle();
    }

    void ACProjectView::onWorldRealized(EventPtr theEvent) {
        AC_DEBUG << "onWorldRealized";
        WindowEventPtr myEvent = boost::static_pointer_cast<WindowEvent>(theEvent);
        if (myEvent->worldname_ == "2dworld") {
            isRealized_ = true;
            if (loadCount_ == 3 || !_myOnlineMode) {
                onAllReady();
            }
        }
        AC_DEBUG << "########################################### world realized: " << myEvent->worldname_;
    }

    //////////////////////////////////////////////////////idle
    const unsigned int ACProjectView::_myKenBurnsDuration = 32000;
    const unsigned int ACProjectView::_myKenBurnsFadeDuration = 4000;
    const float ACProjectView::d = _myKenBurnsDuration + _myKenBurnsFadeDuration;
    const float ACProjectView::rd = d/_myKenBurnsDuration;
    const float ACProjectView::lt = _myKenBurnsFadeDuration/(2.0f*_myKenBurnsDuration);
    const float ACProjectView::ut = 1.0f - lt;

    void ACProjectView::fitToWindowSize(const spark::ImagePtr theImage) {
        vector2 myWindowDimensions = _mySparkWindow->getSize();
        vector2 myTextureSize = theImage->getTextureSize();
        float scaleX = myWindowDimensions[0] / myTextureSize[0];
        float scaleY = myWindowDimensions[1] / myTextureSize[1];
        float scale = std::max(scaleX, scaleY);

        UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(theImage->getShape()->elementList_[0]->material_);
        TextureUnitPtr myTextureUnit = myMaterial->getTextureUnit();
        cml::matrix_scale_2D(myTextureUnit->matrix_, scaleX/scale, scaleY/scale);
        theImage->setSize(myWindowDimensions);
    }

    void ACProjectView::initIdle() {
        AC_DEBUG << "init idle";
        ACProjectViewPtr ptr = boost::static_pointer_cast<ACProjectView>(shared_from_this());
        _myIdleScreenImagePtrs.push_back(boost::static_pointer_cast<spark::Image>(_mySparkWindow->getChildByName("2dworld")->getChildByName("idleimage_1", true)));
        _myIdleScreenImagePtrs.push_back(boost::static_pointer_cast<spark::Image>(_mySparkWindow->getChildByName("2dworld")->getChildByName("idleimage_2", true)));
        fitToWindowSize(_myIdleScreenImagePtrs[0]);
        fitToWindowSize(_myIdleScreenImagePtrs[1]);
        _myIdleDelay = animation::DelayAnimationPtr(new animation::DelayAnimation(_myIdleTime));
        _myIdleDelay->setOnFinish(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::onIdle)));
        spark::EventCallbackPtr myTouchCB = EventCallbackPtr(new ACProjectViewEventCB(ptr, &ACProjectView::onTouch));
        _mySparkWindow->addEventListener(TouchEvent::TAP, myTouchCB);
        _mySparkWindow->addEventListener(GestureEvent::SWIPE_LEFT, myTouchCB);
        _mySparkWindow->addEventListener(GestureEvent::SWIPE_RIGHT, myTouchCB);
        onIdle();
    }

    void ACProjectView::updateKenBurnsShader(float theProgress) {
        ACProjectViewPtr ptr = boost::static_pointer_cast<ACProjectView>(shared_from_this());
        std::map<std::string, float>::iterator it1 = _myIdleScreenImagePtrs[0]->customShaderValues_.find("u_time");
        std::map<std::string, float>::iterator it2 = _myIdleScreenImagePtrs[1]->customShaderValues_.find("u_time");
        float t[2];
        //AC_TRACE << "rd " << rd << " ts " << lt << "|" << ut;
        if (theProgress < lt) {
            swappedIdleImages_ = false;
            t[firstIdleImageVisible_?0:1] = (lt + theProgress)/rd;
            t[firstIdleImageVisible_?1:0] = (1 + lt + theProgress)/rd;
            AC_TRACE << "<lt" << t[0] << "|" << t[1];
        } else if (theProgress > ut && swappedIdleImages_) {
            t[firstIdleImageVisible_?0:1] = (theProgress - ut)/rd;
            t[firstIdleImageVisible_?1:0] = (1 + theProgress - ut)/rd;
            AC_TRACE << ">ut" << t[0] << "|" << t[1];
        } else {
            t[firstIdleImageVisible_?0:1] = (lt + theProgress)/rd;
        }
        if (it1 != _myIdleScreenImagePtrs[0]->customShaderValues_.end()) {
            it1->second = t[0];
        }
        if (it2 != _myIdleScreenImagePtrs[1]->customShaderValues_.end()) {
            it2->second = t[1];
        }
        //AC_TRACE << "updateKenBurnsShader " << theProgress <<  "time " << t[0] << " " << t[2] << "first " << firstIdleImageVisible_;
    }

    void ACProjectView::onKenBurnsImageFadeStart() {
        AC_TRACE << "_____________________________________ fade start, load to " << (firstIdleImageVisible_?1:0);
        if (idleFiles_.size() >0) {
            _myIdleScreenImagePtrs[firstIdleImageVisible_?1:0]->setSrc(idleFiles_[masl::random((size_t)0,idleFiles_.size()-1)]);
        }
        fitToWindowSize(_myIdleScreenImagePtrs[0]);
        fitToWindowSize(_myIdleScreenImagePtrs[1]);
        _myIdleScreenImagePtrs[firstIdleImageVisible_?1:0]->setVisible(true);
        _myIdleScreenImagePtrs[firstIdleImageVisible_?1:0]->setAlpha(0.0f);
        animation::ParallelAnimationPtr myFadeAnimation = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
        myFadeAnimation->add(WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(WidgetWeakPtr(WidgetPtr(_myIdleScreenImagePtrs[firstIdleImageVisible_?1:0])), &Widget::setAlpha, 0, 1, _myKenBurnsFadeDuration)));
        myFadeAnimation->add(WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(WidgetWeakPtr(WidgetPtr(_myIdleScreenImagePtrs[firstIdleImageVisible_?0:1])), &Widget::setAlpha, 1, 0, _myKenBurnsFadeDuration)));
        animation::AnimationManager::get().play(myFadeAnimation);
        std::map<std::string, float>::iterator it = _myIdleScreenImagePtrs[firstIdleImageVisible_?1:0]->customShaderValues_.find("u_mode");
        it->second = masl::random(0.0f, 1.0f);
        firstIdleImageVisible_ = !firstIdleImageVisible_;
        swappedIdleImages_ = true;
    }

    void ACProjectView::onKenBurnsImageFadeEnd() {
        AC_DEBUG << "_________________________________ fade finished";
        _myIdleScreenImagePtrs[firstIdleImageVisible_?1:0]->setVisible(false);
    }

    void ACProjectView::onTouch(spark::EventPtr theEvent) {
        if (_myIdleDelay) {
            _myIdleDelay->cancel();
            AC_DEBUG << "on touch restart idle delay";
            animation::AnimationManager::get().play(_myIdleDelay);
        }
        if (_myKenBurnsAnimation) {
            _myProjectMenu->setVisible(true);
            _myProjectMenu->setSensible(true);            
            _myStartScreenPtr->setVisible(false);
            _myStartScreenPtr->setSensible(false);
            _myKenBurnsAnimation->cancel();
            _myKenBurnsAnimation = animation::ParallelAnimationPtr();//        AC_DEBUG << "clicked on startscreen";
             _myProjectMenu->arrangeProjects();
            WidgetPropertyAnimationPtr myAnimation = WidgetPropertyAnimationPtr(
                  new WidgetPropertyAnimation(WidgetWeakPtr(WidgetPtr(_myStartScreenPtr)), &Widget::setAlpha, 1, 0, 300, animation::EasingFnc(animation::linearTween)));
            animation::ParallelAnimationPtr myParallel = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
            ACProjectViewPtr ptr = boost::static_pointer_cast<ACProjectView>(shared_from_this());
            myAnimation->setOnPlay(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::onStartIdleFade)));
            myAnimation->setOnFinish(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::onFinishIdleFade)));
            myParallel->add(myAnimation);
            animation::AnimationManager::get().play(myParallel);
        }
    }

    void ACProjectView::onIdle() {
        if (_myKenBurnsAnimation) {
            _myKenBurnsAnimation->cancel();
            _myKenBurnsAnimation = animation::ParallelAnimationPtr();
        }
        _myStartScreenPtr->setVisible(true);
        _myStartScreenPtr->setSensible(true);
        _myStartScreenPtr->setAlpha(1.0);
        _myIdleScreenImagePtrs[0]->setVisible(true);
        _myIdleScreenImagePtrs[0]->setAlpha(1.0);
        _myIdleScreenImagePtrs[1]->setVisible(false);
        _myIdleScreenImagePtrs[1]->setAlpha(0.0);
        firstIdleImageVisible_ = true;
        swappedIdleImages_ = true;
        _myProjectViewer->setVisible(false);
        _myProjectViewer->setSensible(false);
        _myProjectMenu->setVisible(false);
        _myProjectMenu->setSensible(false);
        ACProjectViewPtr ptr = boost::static_pointer_cast<ACProjectView>(shared_from_this());
        _myKenBurnsAnimation = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
        _myKenBurnsAnimation->add(ACProjectViewPropertyAnimationPtr(new ACProjectViewPropertyAnimation(ptr, &ACProjectView::updateKenBurnsShader, 0.0f, 1.0f, _myKenBurnsDuration)));
        animation::SequenceAnimationPtr myFadeSequence = animation::SequenceAnimationPtr(new animation::SequenceAnimation());
        animation::DelayAnimationPtr myFadeDelay0 = animation::DelayAnimationPtr(new animation::DelayAnimation(_myKenBurnsFadeDuration/2.0f));
        animation::DelayAnimationPtr myFadeDelay1 = animation::DelayAnimationPtr(new animation::DelayAnimation(_myKenBurnsDuration - _myKenBurnsFadeDuration));
        animation::DelayAnimationPtr myFadeDelay2 = animation::DelayAnimationPtr(new animation::DelayAnimation(_myKenBurnsFadeDuration/2.0f));
        myFadeDelay2->setOnPlay(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::onKenBurnsImageFadeStart)));
        myFadeDelay0->setOnFinish(masl::CallbackPtr(new ACProjectViewCB(ptr, &ACProjectView::onKenBurnsImageFadeEnd)));
        myFadeSequence->add(myFadeDelay0);
        myFadeSequence->add(myFadeDelay1);
        myFadeSequence->add(myFadeDelay2);
        _myKenBurnsAnimation->add(myFadeSequence);
        _myKenBurnsAnimation->setLoop(true);
        animation::AnimationManager::get().play(_myKenBurnsAnimation);
    }
}


