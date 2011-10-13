#include "ACProjectView.h"
#include <cstdlib>

#include <masl/Logger.h>
#include <masl/MobileSDK.h>
#include <masl/numeric_functions.h>

#include <mar/AssetProvider.h>

#include <spark/Window.h>
#include <spark/Event.h>
#include <spark/SparkComponentFactory.h>
#include <spark/AppProvider.h>


#include <animation/AnimationManager.h>
#include <animation/ParallelAnimation.h>
#include <animation/PropertyAnimation.h>
#include <animation/SequenceAnimation.h>
#include <animation/Easing.h>

#include "ACProjectViewComponentMapInitializer.h"

using namespace spark;
using namespace masl;

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
   
    ACProjectView::ACProjectView():BaseApp("ACProjectView"), 
        firstIdleImageVisible_(true), swappedIdleImages_(true), _myAnimatingFlag(false) {
    }

    ACProjectView::~ACProjectView() {
    }

    void ACProjectView::setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, int theScreenWidth, int theScreenHeight) {
        BaseApp::setup(theCurrentMillis, theAssetPath, theScreenWidth, theScreenHeight);
        ACProjectViewComponentMapInitializer::init();        

        loadLayoutAndRegisterEvents("/main", theScreenWidth, theScreenHeight);
        
        ACProjectViewPtr ptr = boost::static_pointer_cast<ACProjectView>(shared_from_this());
        _myProjectMenu =  boost::static_pointer_cast<ProjectMenu>(_mySparkWindow->getChildByName("2dworld")->getChildByName("main",true));
        _myProjectViewer = boost::static_pointer_cast<ProjectViewerImpl>(_mySparkWindow->getChildByName("2dworld")->getChildByName("projectViewer",true));

        spark::RectanglePtr myBackground = boost::static_pointer_cast<Rectangle>(_mySparkWindow->getChildByName("2dworld")->getChildByName("background2"));
        myBackground->setSize(_mySparkWindow->getSize());
                    
        _myProjectMenu->setSensible(false);
        _myStartScreenPtr =  boost::static_pointer_cast<Transform>(_mySparkWindow->getChildByName("2dworld")->getChildByName("startscreen",true));
        spark::EventCallbackPtr startToMenuAniCB = EventCallbackPtr(new ACProjectViewEventCB(ptr, &ACProjectView::onStartScreenClicked));
        _myStartScreenPtr->addEventListener(TouchEvent::PICKED, startToMenuAniCB,true);
        
        spark::EventCallbackPtr myPickedCB = EventCallbackPtr(new ACProjectViewEventCB(ptr, &ACProjectView::onProjectItem));
        spark::EventCallbackPtr myBackCB = EventCallbackPtr(new ACProjectViewEventCB(ptr, &ACProjectView::onBack));
        
        _myProjectViewer->addEventListener(TouchEvent::PICKED, myBackCB,true);
        
        _myProjectItems = boost::static_pointer_cast<spark::Container>(_myProjectMenu);
        const VectorOfComponentPtr & myChildren = _myProjectItems->getChildrenByType(ProjectImpl::SPARK_TYPE);
        for (size_t i = 0; i < myChildren.size(); i++) {
            ProjectImplPtr myProject = boost::static_pointer_cast<ProjectImpl>(myChildren[i]);
            if (myProject) {
                myProject->addEventListener(TouchEvent::PICKED, myPickedCB,true);
            }
        }

        spark::TextPtr myGermanButton = boost::static_pointer_cast<Text>(_mySparkWindow->getChildByName("deutsch", true));
        spark::TextPtr myEnglishButton = boost::static_pointer_cast<Text>(_mySparkWindow->getChildByName("english", true));
        spark::RectanglePtr myRightBar = boost::static_pointer_cast<Rectangle>(_mySparkWindow->getChildByName("right_bar", true));
        spark::RectanglePtr myLangSep = boost::static_pointer_cast<Rectangle>(_mySparkWindow->getChildByName("lang_sep", true));
        myLangSep->setSize(vector2(myLangSep->getShape()->getBoundingBox().max[0], myGermanButton->getTextSize()[1]));
        spark::RectanglePtr myStartBackground = boost::static_pointer_cast<Rectangle>(_myStartScreenPtr->getChildByName("background"));
        myStartBackground->setSize(_mySparkWindow->getSize());
        
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
    }
    
    void ACProjectView::onStartScreenClicked(EventPtr theEvent) {
        AC_DEBUG << "clicked on startscreen";

        _myStartScreenPtr->setSensible(false);
        _myProjectMenu->setSensible(true);
        _myProjectMenu->arrangeProjects();
        WidgetPropertyAnimationPtr myAnimation = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(_myStartScreenPtr, &Widget::setAlpha, 1, 0, 300, animation::EasingFnc(animation::linearTween)));
        animation::ParallelAnimationPtr myParallel = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
            
        ACProjectViewPtr ptr = boost::static_pointer_cast<ACProjectView>(shared_from_this());
        myAnimation->setOnPlay(masl::CallbackPtr(
                         new masl::MemberFunctionCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onStartIdleFade)));
        myAnimation->setOnFinish(masl::CallbackPtr(
                         new masl::MemberFunctionCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onFinishIdleFade)));
            
        myParallel->add(myAnimation);
        animation::AnimationManager::get().play(myParallel);
         

    }
    
    void ACProjectView::onStartIdleFade() {
        _myStartScreenPtr->setVisible(true);
    }

    void ACProjectView::onFinishIdleFade() {
        _myStartScreenPtr->setVisible(false);
    }

    void ACProjectView::onProjectItem(EventPtr theEvent) {
        if (_myAnimatingFlag) {
            return;
        }
        MobileSDK_Singleton::get().getNative()->vibrate(10);                
        _myProjectViewer->setVisible(true);
        _myProjectViewer->setSensible(true);
        _myProjectMenu->setSensible(false);
        _myCurrentProject = boost::static_pointer_cast<ProjectImpl>(theEvent->getTarget()->getParent());
        projectViewAnimation(true);
    }

    
    void ACProjectView::onBack(EventPtr theEvent) {
        if (_myAnimatingFlag || _myProjectMenu->isSensible()) {
            return;
        }
        TouchEventPtr myEvent = boost::static_pointer_cast<TouchEvent>(theEvent);
        if (myEvent->getY() > ProjectViewerImpl::POPUP_HEIGHT * 2 && !_myProjectViewer->isPopUpOpen()) { 
            MobileSDK_Singleton::get().getNative()->vibrate(10);                
            projectViewAnimation(false);
            _myProjectMenu->setSensible(true);
            _myProjectViewer->setSensible(false);
        }
    }
    
    void ACProjectView::onInitiateProjectView() {   
        boost::timer::timer myLoadTimer;            
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
        _myAnimatingFlag = false;        
    }

    void ACProjectView::onFinishProjectView() {        
        _myProjectViewer->setVisible(false);
        _myAnimatingFlag = false;        
    }
    void ACProjectView::onReturn2ProjectView() {        
        _myProjectViewer->showPopup(false);
        _myProjectMenu->setVisible(true);
    }
    
    void ACProjectView::onLoadInitialSet() {        
        _myProjectViewer->loadInitialSet();
    }
    
    
    void ACProjectView::projectViewAnimation(bool showProject){
        int myProjectMenuItemWidth = _myProjectMenu->getIconWidth();
        int myProjectMenuItemHeight = _myProjectMenu->getIconHeight();
        int mySlide = _myProjectMenu->getCurrentSlide();
        _myAnimatingFlag = true;
        animation::SequenceAnimationPtr mySeqAnimation = animation::SequenceAnimationPtr(new animation::SequenceAnimation());
        ACProjectViewPtr ptr = boost::static_pointer_cast<ACProjectView>(shared_from_this());
        int toX = showProject ? 0 : _myCurrentProject->getX()+ myProjectMenuItemWidth/2 - mySlide*_mySparkWindow->getSize()[0];
        int fromX   = showProject ? _myCurrentProject->getX()+ myProjectMenuItemWidth/2  - mySlide*_mySparkWindow->getSize()[0]: 0;
        int toY = showProject ? 0 : _myCurrentProject->getY()+ myProjectMenuItemHeight/2;
        int fromY   = showProject ? _myCurrentProject->getY()+ myProjectMenuItemHeight/2 : 0;
        int toScale = showProject ? 1 :  0;
        int fromScale   = showProject ? 0 :  1;
        
        WidgetPropertyAnimationPtr myZoomAnimationX = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(_myProjectViewer, &Widget::setScaleX, fromScale, toScale, _myAnimationTime,
                    animation::EasingFnc(animation::easeInOutQuad)));
        WidgetPropertyAnimationPtr myZoomAnimationY = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(_myProjectViewer, &Widget::setScaleY, fromScale, toScale, _myAnimationTime,
                    animation::EasingFnc(animation::easeInOutQuad)));
        WidgetPropertyAnimationPtr myTransAnimationX = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(_myProjectViewer, &Widget::setX, fromX, toX, _myAnimationTime,
                    animation::EasingFnc(animation::easeInOutQuad)));
        WidgetPropertyAnimationPtr myTransAnimationY = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(_myProjectViewer, &Widget::setY, fromY, toY, _myAnimationTime,
                    animation::EasingFnc(animation::easeInOutQuad)));
           
        animation::ParallelAnimationPtr myParallel = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
        myParallel->setOnPlay(masl::CallbackPtr(
                         new masl::MemberFunctionCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onStartProjectView)));
        myParallel->setOnFinish(masl::CallbackPtr(
                         new masl::MemberFunctionCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onShowProjectViewPopup)));

            
        myParallel->add(myZoomAnimationX);
        myParallel->add(myZoomAnimationY);
        myParallel->add(myTransAnimationX);
        myParallel->add(myTransAnimationY);

        mySeqAnimation->add(myParallel);
        if (showProject) {                       
            mySeqAnimation->setOnPlay(masl::CallbackPtr(
                            new masl::MemberFunctionCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onInitiateProjectView)));            
            mySeqAnimation->setOnFinish(masl::CallbackPtr(
                            new masl::MemberFunctionCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onFinishLoadProjectView)));
            mySeqAnimation->setOnCancel(masl::CallbackPtr(
                            new masl::MemberFunctionCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onFinishLoadProjectView)));

            animation::DelayAnimationPtr myFrameDelayAnim1 = animation::DelayAnimationPtr(new animation::DelayAnimation(2));
            animation::DelayAnimationPtr myFrameDelayAnim2 = animation::DelayAnimationPtr(new animation::DelayAnimation(20));
            myFrameDelayAnim2->setOnPlay(masl::CallbackPtr(
                            new masl::MemberFunctionCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onLoadInitialSet)));
            mySeqAnimation->add(myFrameDelayAnim1);
            mySeqAnimation->add(myFrameDelayAnim2);
        } else {
            _myProjectViewer->initiateClose();
            mySeqAnimation->setOnFinish(masl::CallbackPtr(
                            new masl::MemberFunctionCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onFinishProjectView)));
            mySeqAnimation->setOnCancel(masl::CallbackPtr(
                            new masl::MemberFunctionCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onFinishProjectView)));
            mySeqAnimation->setOnPlay(masl::CallbackPtr(
                            new masl::MemberFunctionCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onReturn2ProjectView)));

        }

        animation::AnimationManager::get().play(mySeqAnimation);
    }

    void ACProjectView::onLanguageSwitchDe(EventPtr theEvent) {
        _mySparkWindow->switchLanguage(spark::DE);
    }
    void ACProjectView::onLanguageSwitchEn(EventPtr theEvent) {
        _mySparkWindow->switchLanguage(spark::EN);
    }
    
    void ACProjectView::onWorldRealized(EventPtr theEvent) {
        WindowEventPtr myEvent = boost::static_pointer_cast<WindowEvent>(theEvent);
        if (myEvent->worldname_ == "2dworld") {
            boost::static_pointer_cast<View>(_mySparkWindow->getChildByName("loaderView"))->setVisible(false);
            boost::static_pointer_cast<View>(_mySparkWindow->getChildByName("mainView"))->setVisible(true);
        }
        AC_DEBUG << "######################### world realized: " << myEvent->worldname_;
        if (myEvent->worldname_ == "2dworld") {
            initIdle();
        }
    }

    //////////////////////////////////////////////////////idle
    const unsigned int ACProjectView::_myIdleTime = 20000;
    const unsigned int ACProjectView::_myKenBurnsDuration = 32000;
    const unsigned int ACProjectView::_myKenBurnsFadeDuration = 4000;
    const float ACProjectView::d = _myKenBurnsDuration + _myKenBurnsFadeDuration;
    const float ACProjectView::rd = d/_myKenBurnsDuration;
    const float ACProjectView::lt = _myKenBurnsFadeDuration/(2.0f*_myKenBurnsDuration);
    const float ACProjectView::ut = 1.0f - lt;

    void ACProjectView::initIdle() {
        AC_DEBUG << "init idle";
        ACProjectViewPtr ptr = boost::static_pointer_cast<ACProjectView>(shared_from_this());
        _myIdleScreenImagePtrs.push_back(boost::static_pointer_cast<spark::Image>(_mySparkWindow->getChildByName("2dworld")->getChildByName("idleimage_1", true)));
        _myIdleScreenImagePtrs.push_back(boost::static_pointer_cast<spark::Image>(_mySparkWindow->getChildByName("2dworld")->getChildByName("idleimage_2", true)));
        vector2 myWindowDimensions = _mySparkWindow->getSize();
        _myIdleScreenImagePtrs[0]->fitToSize(myWindowDimensions[0], myWindowDimensions[1]);
        _myIdleScreenImagePtrs[1]->fitToSize(myWindowDimensions[0], myWindowDimensions[1]);
        _myIdleDelay = animation::DelayAnimationPtr(new animation::DelayAnimation(_myIdleTime));
        _myIdleDelay->setOnFinish(masl::CallbackPtr(new masl::MemberFunctionCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onIdle)));
        spark::EventCallbackPtr myTouchCB = EventCallbackPtr(new MemberFunctionEventCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onTouch));
        _mySparkWindow->addEventListener(TouchEvent::TAP, myTouchCB);
        _mySparkWindow->addEventListener(GestureEvent::SWIPE_LEFT, myTouchCB);
        _mySparkWindow->addEventListener(GestureEvent::SWIPE_RIGHT, myTouchCB);
        masl::getDirectoryEntries(mar::AssetProviderSingleton::get().ap()->getAssetPath() + "/textures/large_images/", idleFiles_, "");
        onIdle();
    }

    void ACProjectView::updateKenBurnsShader(float theProgress) {
        ACProjectViewPtr ptr = boost::static_pointer_cast<ACProjectView>(shared_from_this());
        std::map<std::string, float>::iterator it1 = _myIdleScreenImagePtrs[0]->customShaderValues_.find("a_time");
        std::map<std::string, float>::iterator it2 = _myIdleScreenImagePtrs[1]->customShaderValues_.find("a_time");
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
        _myIdleScreenImagePtrs[firstIdleImageVisible_?1:0]->setSrc("/large_images/"+idleFiles_[masl::random((size_t)0,idleFiles_.size()-1)]);
        vector2 myWindowDimensions = _mySparkWindow->getSize();
        _myIdleScreenImagePtrs[0]->fitToSize(myWindowDimensions[0], myWindowDimensions[1]);
        _myIdleScreenImagePtrs[1]->fitToSize(myWindowDimensions[0], myWindowDimensions[1]);
        _myIdleScreenImagePtrs[firstIdleImageVisible_?1:0]->setVisible(true);
        _myIdleScreenImagePtrs[firstIdleImageVisible_?1:0]->setAlpha(0.0f);
        animation::ParallelAnimationPtr myFadeAnimation = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
        myFadeAnimation->add(WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(_myIdleScreenImagePtrs[firstIdleImageVisible_?1:0], &Widget::setAlpha, 0, 1, _myKenBurnsFadeDuration)));
        myFadeAnimation->add(WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(_myIdleScreenImagePtrs[firstIdleImageVisible_?0:1], &Widget::setAlpha, 1, 0, _myKenBurnsFadeDuration)));
        animation::AnimationManager::get().play(myFadeAnimation);
        std::map<std::string, float>::iterator it = _myIdleScreenImagePtrs[firstIdleImageVisible_?1:0]->customShaderValues_.find("a_mode");
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
            _myKenBurnsAnimation = animation::ParallelAnimationPtr();
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
        myFadeDelay2->setOnPlay(masl::CallbackPtr(new masl::MemberFunctionCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onKenBurnsImageFadeStart)));
        myFadeDelay0->setOnFinish(masl::CallbackPtr(new masl::MemberFunctionCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onKenBurnsImageFadeEnd)));
        myFadeSequence->add(myFadeDelay0);
        myFadeSequence->add(myFadeDelay1);
        myFadeSequence->add(myFadeDelay2);
        _myKenBurnsAnimation->add(myFadeSequence);
        _myKenBurnsAnimation->setLoop(true);
        animation::AnimationManager::get().play(_myKenBurnsAnimation);
    }
}


