#include "ACProjectView.h"
#include <cstdlib>

#include <masl/Logger.h>
#include <masl/MobileSDK.h>

#include <spark/Window.h>
#include <spark/Event.h>
#include <spark/SparkComponentFactory.h>
#include <spark/AppProvider.h>


#include <animation/AnimationManager.h>
#include <animation/ParallelAnimation.h>
#include <animation/SequenceAnimation.h>
#include <animation/DelayAnimation.h>
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
   
    ACProjectView::ACProjectView():BaseApp("ACProjectView") {
        _myChooseLayoutFlag = true;
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

        spark::RectanglePtr myBackground = boost::static_pointer_cast<Rectangle>(_mySparkWindow->getChildByName("2dworld")->getChildByName("background"));
        myBackground->setSize(_mySparkWindow->getSize());
                    
        _myWidth = _myProjectMenu->getPreviewWidth();
        _myHeight = _myProjectMenu->getPreviewHeight();
        
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
        spark::RectanglePtr myLangSep = boost::static_pointer_cast<Rectangle>(_mySparkWindow->getChildByName("lang_sep", true));
        myLangSep->setSize(vector2(myLangSep->getShape()->getBoundingBox().max[0], myGermanButton->getTextSize()[1]));
        spark::RectanglePtr myStartBackground = boost::static_pointer_cast<Rectangle>(_myStartScreenPtr->getChildByName("background"));
        myStartBackground->setSize(_mySparkWindow->getSize());
        
        myEnglishButton->setX(_mySparkWindow->getSize()[0] - myEnglishButton->getTextSize()[0] - 12);
        myLangSep->setX( myEnglishButton->getX() - 12);
        myGermanButton->setX( myLangSep->getX() - myGermanButton->getTextSize()[0] - 12);
        
        spark::EventCallbackPtr mySwitchLanguageDeCB = EventCallbackPtr(new ACProjectViewEventCB(ptr, &ACProjectView::onLanguageSwitchDe));
        myGermanButton->addEventListener(TouchEvent::PICKED, mySwitchLanguageDeCB);

        spark::EventCallbackPtr mySwitchLanguageEnCB = EventCallbackPtr(new ACProjectViewEventCB(ptr, &ACProjectView::onLanguageSwitchEn));
        myEnglishButton->addEventListener(TouchEvent::PICKED, mySwitchLanguageEnCB);
    }
    
    void ACProjectView::onStartScreenClicked(EventPtr theEvent) {
        AC_PRINT << "clicked on startscreen";
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
        AC_PRINT << "clicked on project: "<< theEvent->getTarget()->getParent()->getName();
        MobileSDK_Singleton::get().getNative()->vibrate(10);                
        _myProjectViewer->setVisible(true);
        _myProjectViewer->setSensible(true);
        _myProjectMenu->setSensible(false);
        _myCurrentProject = boost::static_pointer_cast<ProjectImpl>(theEvent->getTarget()->getParent());
        projectViewAnimation(true);
    }

    
    void ACProjectView::onBack(EventPtr theEvent) {
        if (_myProjectMenu->isSensible()) return;
        MobileSDK_Singleton::get().getNative()->vibrate(10);                
        projectViewAnimation(false);
        _myProjectMenu->setSensible(true);
        _myProjectViewer->setSensible(false);
    }
    
    void ACProjectView::onInitiateProjectView() {        
        _myProjectViewer->showProject(_myCurrentProject);
    }

    void ACProjectView::onStartProjectView() {        
        _myProjectViewer->setVisible(true);
    }    
    void ACProjectView::onFinishLoadProjectView() {        
        _myProjectViewer->loadInitialSet();
    }
    void ACProjectView::onFinishProjectView() {        
        _myProjectViewer->setVisible(false);
    }
    
    
    void ACProjectView::projectViewAnimation(bool showProject){
        animation::SequenceAnimationPtr mySeqAnimation = animation::SequenceAnimationPtr(new animation::SequenceAnimation());
        ACProjectViewPtr ptr = boost::static_pointer_cast<ACProjectView>(shared_from_this());
        if (showProject) {               
            animation::DelayAnimationPtr myInitiateProjectViewAnim = animation::DelayAnimationPtr(new animation::DelayAnimation(40));
            myInitiateProjectViewAnim->setOnPlay(masl::CallbackPtr(
                            new masl::MemberFunctionCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onInitiateProjectView)));
            mySeqAnimation->add(myInitiateProjectViewAnim);
        }
  
        int toX = showProject ? 0 : _myCurrentProject->getX()+_myWidth/2;
        int fromX   = showProject ? _myCurrentProject->getX()+_myWidth/2 : 0;
        int toY = showProject ? 0 : _myCurrentProject->getY()+_myHeight/2;
        int fromY   = showProject ? _myCurrentProject->getY()+_myHeight/2 : 0;
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
            
        myParallel->add(myZoomAnimationX);
        myParallel->add(myZoomAnimationY);
        myParallel->add(myTransAnimationX);
        myParallel->add(myTransAnimationY);

        mySeqAnimation->add(myParallel);
        if (showProject) {                       
            mySeqAnimation->setOnFinish(masl::CallbackPtr(
                            new masl::MemberFunctionCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onFinishLoadProjectView)));
        } else {
            _myProjectViewer->initiateClose();
            mySeqAnimation->setOnFinish(masl::CallbackPtr(
                            new masl::MemberFunctionCallback<ACProjectView, ACProjectViewPtr>(ptr, &ACProjectView::onFinishProjectView)));

        }
        animation::AnimationManager::get().play(mySeqAnimation);
    }

    void ACProjectView::onLanguageSwitchDe(EventPtr theEvent) {
        _mySparkWindow->switchLanguage(spark::DE);
    }
    void ACProjectView::onLanguageSwitchEn(EventPtr theEvent) {
        _mySparkWindow->switchLanguage(spark::EN);
    }

}


