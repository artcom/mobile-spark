#include "DemoApp.h"

#include <boost/smart_ptr/shared_ptr.hpp>

#include <masl/Logger.h>
#include <masl/MobileSDK_Singleton.h>

#include <animation/AnimationManager.h>
#include <animation/ParallelAnimation.h>
#include <animation/SequenceAnimation.h>
#include <animation/DelayAnimation.h>
#include <animation/Easing.h>

#include "Event.h"
#include "Rectangle.h"
#include "Transform.h"
#include "Shape3D.h"
#include "Text.h"
#include "SparkComponentFactory.h"

/////////////////////////////////////////////////////////////////////////App-Instance
spark::BaseAppPtr ourApp = boost::shared_ptr<spark::DemoApp>(new spark::DemoApp());

/////////////////// Application code, this should be in java or script language later...
namespace spark {

    DemoApp::DemoApp():BaseApp(), _myCurrentSlide(0) {
    }

    DemoApp::~DemoApp() {
    }

    void freeFunction() {
        AC_PRINT << "hello from free function";
    }
    void freeFunctionEventCB(EventPtr theEvent) {
        AC_PRINT << "hello from free function event callback for eventtype " << theEvent->getType();
    }

    bool DemoApp::setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, const std::string & theLayoutFile) {
        bool myBaseReturn = BaseApp::setup(theCurrentMillis, theAssetPath, theLayoutFile);

        ComponentPtr my2DWorld = _mySparkWindow->getChildByName("2dworld");

        //test free function on touch
        spark::EventCallbackPtr myFreeCB = EventCallbackPtr(new FreeFunctionEventCallback(freeFunctionEventCB));
        _mySparkWindow->addEventListener(TouchEvent::TAP, myFreeCB);
        
        //button callbacks
        DemoAppPtr ptr = boost::static_pointer_cast<DemoApp>(shared_from_this());
        spark::EventCallbackPtr myPickedCB = EventCallbackPtr(new DemoEventCB(ptr, &DemoApp::onControlButton));
        ComponentPtr myBackButton = my2DWorld->getChildByName("backbutton", true);
        ComponentPtr myNextButton = my2DWorld->getChildByName("nextbutton", true);
        myBackButton->addEventListener(TouchEvent::PICKED, myPickedCB);
        myNextButton->addEventListener(TouchEvent::PICKED, myPickedCB);
        spark::EventCallbackPtr myCreationCB = EventCallbackPtr(new DemoEventCB(ptr, &DemoApp::onCreationButton));
        ComponentPtr myCreationButton = my2DWorld->getChildByName("creationbutton", true);
        myCreationButton->addEventListener(TouchEvent::PICKED, myCreationCB);
        spark::EventCallbackPtr myAnimationCB = EventCallbackPtr(new DemoEventCB(ptr, &DemoApp::onTouch));
        _mySparkWindow->addEventListener(TouchEvent::TAP, myAnimationCB);

        WidgetPropertyAnimationPtr myXRotate, myYRotate, myZRotate;
        //animation of amazone
        ComponentPtr myComponent = _mySparkWindow->getChildByName("3dworld")->getChildByName("transform")->getChildByName("theAmazone");
        Shape3DPtr myShape = boost::static_pointer_cast<spark::Shape3D>(myComponent);
        myYRotate = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myShape, &Widget::setRotationY, 0, M_PI * 2, 90000));
        myYRotate->setLoop(true);
        animation::AnimationManager::get().play(myYRotate);

        //animation of 3d object
        myComponent = my2DWorld->getChildByName("funnyShape", true);
        myShape = boost::static_pointer_cast<spark::Shape3D>(myComponent);
        myXRotate = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myShape, &Widget::setRotationX, 0, 6.28, 7000));
        myXRotate->setLoop(true);
        myYRotate = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myShape, &Widget::setRotationY, 0, 6.28, 9000));
        myYRotate->setLoop(true);
        myZRotate = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myShape, &Widget::setRotationZ, 0, 6.28, 13000));
        myZRotate->setLoop(true);
        animation::AnimationManager::get().play(myXRotate);
        animation::AnimationManager::get().play(myYRotate);
        animation::AnimationManager::get().play(myZRotate);

        //add looping sequence animation for rectangle
        myComponent = my2DWorld->getChildByName("AnimatedRectangleSlide")->getChildByName("transform")->getChildByName("redObject");
        RectanglePtr myRectangle = boost::static_pointer_cast<spark::Rectangle>(myComponent);
        WidgetPropertyAnimationPtr myAnimation1 = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myRectangle, &Widget::setScaleY, 0.7, 8, 500));
        WidgetPropertyAnimationPtr myAnimation2 = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(myRectangle, &Widget::setScaleY, 8, 0.7, 1500, animation::EasingFnc(animation::easeInOutQuint)));
        animation::DelayAnimationPtr myDelay = animation::DelayAnimationPtr(new animation::DelayAnimation(2000));
        animation::SequenceAnimationPtr mySequence = animation::SequenceAnimationPtr(new animation::SequenceAnimation());
        mySequence->add(myAnimation1);
        mySequence->add(myAnimation2);
        mySequence->add(myDelay);
        mySequence->setLoop(true);
        //mySequence->setOnPlay(masl::CallbackPtr(new masl::MemberFunctionCallback<Widget, RectanglePtr>( myRectangle, &Widget::test)));
        //mySequence->setOnFinish(masl::CallbackPtr(new masl::FreeFunctionCallback(freeFunction)));
        animation::AnimationManager::get().play(mySequence);

        //collect slides and views
        ContainerPtr myContainer = boost::static_pointer_cast<spark::Container>(my2DWorld);
        const VectorOfComponentPtr & myChildren = myContainer->getChildrenByType("SlideImpl");
        for (size_t i = 0; i < myChildren.size(); i++) {
            SlideImplPtr mySlide = boost::static_pointer_cast<spark::SlideImpl>(myChildren[i]);
            if (mySlide) {
                _mySlides.push_back(mySlide);
                mySlide->setVisible(false);
                mySlide->setSensible(false);
            }
            AC_DEBUG << "add slide to slides : " << mySlide->getName();
        }
        const VectorOfComponentPtr & myWindowChildren = _mySparkWindow->getChildrenByType(View::SPARK_TYPE);
        for (size_t i = 0; i < myWindowChildren.size(); i++) {
            ViewPtr myView = boost::static_pointer_cast<spark::View>(myWindowChildren[i]);
            if (myView) {
                _myViews.push_back(myView);
                myView->setVisible(true);
            }
            AC_DEBUG << "add view to views : " << myView->getName();
        }
        _myCurrentSlide = 0;
        _mySlides[_myCurrentSlide]->setVisible(true);

        AC_DEBUG << "found #" << _mySlides.size() << " slides";
        
        return myBaseReturn;
    }

    void DemoApp::onControlButton(EventPtr theEvent) {
        AC_DEBUG << "on control button";
        _mySlides[_myCurrentSlide]->setVisible(false);
        _mySlides[_myCurrentSlide]->setSensible(false);
        _myCurrentSlide = (_myCurrentSlide + _mySlides.size() + 
                          ( theEvent->getTarget()->getName() == "backbutton" ? -1 : +1)) % _mySlides.size();
        AC_DEBUG << ">>>>> activate slide: " << _mySlides[_myCurrentSlide]->getName();
        _mySlides[_myCurrentSlide]->setVisible(true);        
        _mySlides[_myCurrentSlide]->setSensible(true);        
    }

    void DemoApp::onCreationButton(EventPtr theEvent) {
        AC_DEBUG << "on creation button";
        ComponentPtr myTransform = _mySparkWindow->getChildByName("2dworld")->getChildByName("ObjectCreationSlide");
        ComponentPtr myCreated = myTransform->getChildByName("created_from_code");
        if (myCreated) {
            boost::static_pointer_cast<spark::Container>(myTransform)->removeChild(myCreated);
        } else {
            //TRICK: this has to be done on main thread -> use delay animation with 0 duration achieve this
            animation::DelayAnimationPtr myDelay = animation::DelayAnimationPtr(new animation::DelayAnimation(0));
            DemoAppPtr ptr = boost::static_pointer_cast<DemoApp>(shared_from_this());
            myDelay->setOnFinish(masl::CallbackPtr(
                        new masl::MemberFunctionCallback<DemoApp, DemoAppPtr>(ptr, &DemoApp::insertCreatedComponent)));
            animation::AnimationManager::get().play(myDelay);
        }
    }

    void DemoApp::insertCreatedComponent() {
        ComponentPtr myTransform = _mySparkWindow->getChildByName("2dworld")->getChildByName("ObjectCreationSlide");
        ComponentPtr myCreated = SparkComponentFactory::get().loadSparkComponentsFromString(shared_from_this(), 
                "<Rectangle name=\"created_from_code\" width=\"300\" height=\"10\" color=\"[1.0,1.0,0.0]\"/>"); 
        myCreated->insertAtParent(boost::static_pointer_cast<spark::Container>(myTransform));
    }

    void DemoApp::onTouch(EventPtr theEvent) {
        TouchEventPtr myEvent = boost::static_pointer_cast<TouchEvent>(theEvent);
        AC_DEBUG << "on touch "<<myEvent->getType()<<" x:"<<myEvent->getX()<<" ,y:"<<myEvent->getY();
        //add parallel animations
        ComponentPtr mySlide = _mySparkWindow->getChildByName("2dworld")->getChildByName("AnimatedRectangleSlide");
        ComponentPtr myObject = mySlide->getChildByName("greenObject");
        RectanglePtr myRectangle = boost::static_pointer_cast<spark::Rectangle>(myObject);
        WidgetPropertyAnimationPtr myAnimationX = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(myRectangle, &Widget::setX, myRectangle->getX(), myEvent->getX(), 1000,
                    animation::EasingFnc(animation::easeInOutElastic)));
        WidgetPropertyAnimationPtr myAnimationY = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(myRectangle, &Widget::setY, myRectangle->getY(), myEvent->getY(), 1000,
                    animation::EasingFnc(animation::easeInOutElastic)));
        myObject = mySlide->getChildByName("transform")->getChildByName("blueObject");
        myRectangle = boost::static_pointer_cast<spark::Rectangle>(myObject);
        WidgetPropertyAnimationPtr myRotationAnimation = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myRectangle, &Widget::setRotationZ, 0, M_PI * 2, 5000));
        animation::ParallelAnimationPtr myParallel = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
        myParallel->add(myAnimationX);
        myParallel->add(myAnimationY);
        myParallel->add(myRotationAnimation);
        animation::AnimationManager::get().play(myParallel);
    }
    
    void
    DemoApp::centerSlideTitlesToNewCanvasSize(int theWidth, int theHeight) {
        for (std::vector<SlideImplPtr>::iterator it = _mySlides.begin(); it != _mySlides.end(); ++it) {
            (*it)->centerTitles(theWidth, theHeight);
        }
    }
    
    void DemoApp::onSizeChanged(int theWidth, int theHeight) {
        BaseApp::onSizeChanged(theWidth, theHeight);
        centerSlideTitlesToNewCanvasSize(theWidth, theHeight);
    }
}


