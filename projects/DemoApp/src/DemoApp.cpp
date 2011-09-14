#include "DemoApp.h"

#include <boost/smart_ptr/shared_ptr.hpp>

#include <masl/Logger.h>
#include <masl/MobileSDK_Singleton.h>

#include <animation/AnimationManager.h>
#include <animation/ParallelAnimation.h>
#include <animation/SequenceAnimation.h>
#include <animation/DelayAnimation.h>
#include <animation/Easing.h>

#include <spark/Event.h>
#include <spark/Rectangle.h>
#include <spark/Transform.h>
#include <spark/Shape3D.h>
#include <spark/Text.h>
#include <spark/SparkComponentFactory.h>
#include <spark/AppProvider.h>

using namespace spark;
using namespace masl;

/////////////////////////////////////////////////////////////////////////App-Instance
#ifdef __ANDROID__
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    spark::AppProvider::get().setApp(boost::shared_ptr<demoapp::DemoApp>(new demoapp::DemoApp()));
    return JNI_VERSION_1_6;
}
#endif


/////////////////// Application code, this should be in java or script language later...
namespace demoapp {

    DemoApp::DemoApp():BaseApp("demoapp"), _myCurrentSlide(0) {
    }

    DemoApp::~DemoApp() {
    }

    void freeFunction() {
        AC_PRINT << "hello from free function";
    }
    void freeFunctionEventCB(EventPtr theEvent) {
        AC_PRINT << "hello from free function event callback for eventtype " << theEvent->getType();
    }

    void DemoApp::setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, int theScreenWidth, int theScreenHeight) {
        BaseApp::setup(theCurrentMillis, theAssetPath, theScreenWidth, theScreenHeight);
        AC_PRINT << "################# DemoApp::setup : " << theScreenWidth << "/" << theScreenHeight;
        
        std::string myOrientation;
        std::string mySparkFile = findBestMatchedLayout("/main", theScreenWidth, theScreenHeight, myOrientation);
        AC_PRINT << "################# DmyOrientation : " << myOrientation;
        MobileSDK_Singleton::get().freezeMobileOrientation(myOrientation);
                    
        loadLayoutAndRegisterEvents(mySparkFile);

        DemoAppPtr ptr = boost::static_pointer_cast<DemoApp>(shared_from_this());
        EventCallbackPtr mySizeChangedCB = EventCallbackPtr(new DemoEventCB(ptr, &DemoApp::onSizeChanged));
        _mySparkWindow->addEventListener(WindowEvent::ON_RESIZE, mySizeChangedCB);

        ComponentPtr my2DWorld = _mySparkWindow->getChildByName("2dworld");

        //test free function on touch
        spark::EventCallbackPtr myFreeCB = EventCallbackPtr(new FreeFunctionEventCallback(freeFunctionEventCB));
        _mySparkWindow->addEventListener(TouchEvent::TAP, myFreeCB);

        //button callbacks
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

		//test swipe gestures
        spark::EventCallbackPtr mySwipeCB = EventCallbackPtr(new DemoEventCB(ptr,&DemoApp::onSwipeGesture));
        _mySparkWindow->addEventListener(GestureEvent::SWIPE_LEFT, mySwipeCB);
		_mySparkWindow->addEventListener(GestureEvent::SWIPE_RIGHT, mySwipeCB);

		//test pinch gestures
        spark::EventCallbackPtr myPinchCB = EventCallbackPtr(new DemoEventCB(ptr,&DemoApp::onPinchGesture));
        _mySparkWindow->addEventListener(GestureEvent::PINCH, myPinchCB);
        
        //test pan gestures
        spark::EventCallbackPtr myPanCB = EventCallbackPtr(new DemoEventCB(ptr,&DemoApp::onPanGesture));
        _mySparkWindow->addEventListener(GestureEvent::PAN, myPanCB);
        
        //test sensors
        spark::EventCallbackPtr mySensorCB = EventCallbackPtr(new DemoEventCB(ptr,&DemoApp::onSensorEvent));
        _mySparkWindow->addEventListener(SensorEvent::ORIENTATION, mySensorCB);
        spark::EventCallbackPtr mySensorLightCB = EventCallbackPtr(new DemoEventCB(ptr,&DemoApp::onSensorLightEvent));
        _mySparkWindow->addEventListener(SensorEvent::LIGHT, mySensorLightCB);
        spark::EventCallbackPtr mySensorGyroCB = EventCallbackPtr(new DemoEventCB(ptr,&DemoApp::onSensorGyroEvent));
        _mySparkWindow->addEventListener(SensorEvent::GYROSCOPE, mySensorGyroCB);



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
            SlideImplPtr mySlide = boost::static_pointer_cast<SlideImpl>(myChildren[i]);
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
        
        //return myOrientation;
    }

    void DemoApp::onControlButton(EventPtr theEvent) {
        AC_DEBUG << "on control button";
    	changeSlide(theEvent->getTarget()->getName() == "backbutton" ? -1 :  +1);    }
    
    void DemoApp::onStartSlideSwipe() {
        _mySlides[_myNextSlide]->setVisible(true);
        _mySlides[_myNextSlide]->setX(_mySparkWindow->getSize()[0]);        
    }
    void DemoApp::onFinishSlideSwipe() {
        _mySlides[_myCurrentSlide]->setVisible(false);
        _mySlides[_myCurrentSlide]->setSensible(false);
        _myCurrentSlide = _myNextSlide;
        _mySlides[_myCurrentSlide]->setSensible(true);
    }
    
    void DemoApp::onSwipeGesture(EventPtr theEvent) {
    	AC_DEBUG << "on Swipe Gesture";
    	changeSlide(theEvent->getType() == "swipe-right" ? -1 :  +1);
    }
    
    void DemoApp::changeSlide(int theDirection) {
        DemoAppPtr ptr = boost::static_pointer_cast<DemoApp>(shared_from_this());    	
        animation::ParallelAnimationPtr mySequence = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
        mySequence->setOnPlay(masl::CallbackPtr(
                    new masl::MemberFunctionCallback<DemoApp, DemoAppPtr>(ptr, &DemoApp::onStartSlideSwipe)));
        WidgetPropertyAnimationPtr mySwipeOut = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(_mySlides[_myCurrentSlide], 
                                                &Widget::setX, 0, _mySparkWindow->getSize()[0] * theDirection * -1, 500,
                                                animation::EasingFnc(animation::easeInOutQuad)));
        _myNextSlide = (_myCurrentSlide + _mySlides.size() + theDirection) % _mySlides.size();
        WidgetPropertyAnimationPtr mySwipeIn = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(_mySlides[_myNextSlide], 
                                                &Widget::setX, _mySparkWindow->getSize()[0] * theDirection, 0, 500,
                                                animation::EasingFnc(animation::easeInOutQuad)));
        mySequence->setOnFinish(masl::CallbackPtr(
                    new masl::MemberFunctionCallback<DemoApp, DemoAppPtr>(ptr, &DemoApp::onFinishSlideSwipe)));
    	mySequence->add(mySwipeOut);
    	mySequence->add(mySwipeIn);
        animation::AnimationManager::get().play(mySequence);
    }
    
    void DemoApp::onPinchGesture(EventPtr theEvent) {
        if (_mySlides[_myCurrentSlide]->getName() =="3D-Viewer-Slide") {
        	AC_DEBUG << "on Pinch Gesture";
        	GestureEventPtr myEvent = boost::static_pointer_cast<GestureEvent>(theEvent);
        	float myScaleFactor = myEvent->getFactor(); 
        	ComponentPtr my3dView = _mySparkWindow->getChildByName("3dworld");
            boost::static_pointer_cast<Widget>(my3dView)->setScaleX(myScaleFactor);
    		boost::static_pointer_cast<Widget>(my3dView)->setScaleY(myScaleFactor);
		}
    }
    
    void DemoApp::onPanGesture(EventPtr theEvent) {
        if (_mySlides[_myCurrentSlide]->getName() =="3D-Viewer-Slide") {
        	AC_DEBUG << "on Pan Gesture";
        	GestureEventPtr myEvent = boost::static_pointer_cast<GestureEvent>(theEvent);
        	float myDX = myEvent->getTranslateX();
        	float myDY = myEvent->getTranslateY(); 
        	float myX = myEvent->getX() - _mySparkWindow->getSize()[0] / 2;
        	float myY = myEvent->getY() - _mySparkWindow->getSize()[1] / 2; 
        	ComponentPtr my3dView = _mySparkWindow->getChildByName("3dworld");
            boost::static_pointer_cast<Widget>(my3dView)->setX((myX + myDX) / 3);
    		boost::static_pointer_cast<Widget>(my3dView)->setY((myY + myDY) / 3);
		}
    }

	void DemoApp::onSensorEvent(EventPtr theEvent) {
    	SensorEventPtr myEvent = boost::static_pointer_cast<SensorEvent>(theEvent);
    	float myY = myEvent->getValue1(); 
    	float myZ = myEvent->getValue2();
    	ComponentPtr myWhiteObject = _mySparkWindow->getChildByName("transformOrient", true);
        TransformPtr myWhiteRectangle = boost::static_pointer_cast<spark::Transform>(myWhiteObject);
		myWhiteRectangle->setX(myY*4+200);
		myWhiteRectangle->setY(myZ*4+200);
    }

	void DemoApp::onSensorLightEvent(EventPtr theEvent) {
    	SensorEventPtr myEvent = boost::static_pointer_cast<SensorEvent>(theEvent);
    	float myLight = myEvent->getValue0();
       	ComponentPtr myLightObject = _mySparkWindow->getChildByName("transformLight", true);
        TransformPtr myLightRectangle = boost::static_pointer_cast<spark::Transform>(myLightObject);
		myLightRectangle->setY(myLight/10);
    }
    
    void DemoApp::onSensorGyroEvent(EventPtr theEvent) {
    	SensorEventPtr myEvent = boost::static_pointer_cast<SensorEvent>(theEvent);
    	float myX = myEvent->getValue0();
       	float myY = myEvent->getValue1();
       	ComponentPtr myGyroObject = _mySparkWindow->getChildByName("gyro", true);
        RectanglePtr myGyroRectangle = boost::static_pointer_cast<spark::Rectangle>(myGyroObject);
		myGyroRectangle->setX(myX*20-15);
		myGyroRectangle->setY(myY*20-15);
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
    
    void DemoApp::onSizeChanged(EventPtr theEvent) {
        WindowEventPtr myEvent = boost::static_pointer_cast<WindowEvent>(theEvent);            
        centerSlideTitlesToNewCanvasSize(myEvent->size_[0], myEvent->size_[1]);
    }
}


