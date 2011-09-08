#include "DemoApp.h"

#ifdef __ANDROID__
    #include <jni.h>
#endif

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
#include "SparkComponentFactory.h"

/////////////////////////////////////////////////////////////////////////App-Instance
spark::DemoApp ourApp;

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

        //test free function on touch
        spark::EventCallbackPtr myFreeCB = EventCallbackPtr(new FreeFunctionEventCallback(freeFunctionEventCB));
        _mySparkWindow->addEventListener(TouchEvent::TAP, myFreeCB);

        DemoAppPtr ptr = boost::static_pointer_cast<DemoApp>(shared_from_this());
        spark::EventCallbackPtr myPickedCB = EventCallbackPtr(new MemberFunctionEventCallback<DemoApp, DemoAppPtr> (ptr, &DemoApp::onNextButton));
        ComponentPtr myNextButton = _mySparkWindow->getChildByName("nextbutton", true);
        myNextButton->addEventListener(TouchEvent::PICKED, myPickedCB);

        //animation of amazone
        ComponentPtr myComponent = _mySparkWindow->getChildByName("3dworld")->getChildByName("transform")->getChildByName("theAmazone");
        Shape3DPtr myShape = boost::static_pointer_cast<spark::Shape3D>(myComponent);
        WidgetPropertyAnimationPtr myYRotate = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myShape, &Widget::setRotationY, 0, M_PI * 2, 90000));
        myYRotate->setLoop(true);
        animation::AnimationManager::get().play(myYRotate);

        ComponentPtr my2DWorld = _mySparkWindow->getChildByName("2dworld");
        ContainerPtr myContainer = boost::static_pointer_cast<spark::Container>(my2DWorld);
        const VectorOfComponentPtr & myChildren = myContainer->getChildrenByType(Transform::SPARK_TYPE);
        for (size_t i = 0; i < myChildren.size(); i++) {
            TransformPtr myTransform = boost::static_pointer_cast<spark::Transform>(myChildren[i]);
            if (myTransform) {
                _mySlides.push_back(myTransform);
                myTransform->setVisible(false);
                myTransform->setSensible(false);
            }
            AC_PRINT << "add slide to slides : " << myTransform->getName();
        }
        
        const VectorOfComponentPtr & myWindowChildren = _mySparkWindow->getChildrenByType(View::SPARK_TYPE);
        for (size_t i = 0; i < myWindowChildren.size(); i++) {
            ViewPtr myView = boost::static_pointer_cast<spark::View>(myWindowChildren[i]);
            if (myView) {
                _myViews.push_back(myView);
                myView->setVisible(true);
            }
            AC_PRINT << "add view to views : " << myView->getName();
        }
        _myCurrentSlide = 0;
        _mySlides[_myCurrentSlide]->setVisible(true);
        AC_PRINT << "found #" << _mySlides.size() << " slides";
        return myBaseReturn;


        //TODO
/*
        //add looping animations
        ComponentPtr myTransform = _mySparkWindow->getChildByName("world1")->getChildByName("transformB");
        ComponentPtr myObject = myTransform->getChildByName("objectB");
        RectanglePtr myRectangle = boost::static_pointer_cast<spark::Rectangle>(myObject);
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

        myTransform = _mySparkWindow->getChildByName("world1")->getChildByName("transformA")->getChildByName("objTransform");
        myObject = myTransform->getChildByName("objShape");
        Shape3DPtr myShape = boost::static_pointer_cast<spark::Shape3D>(myObject);
        WidgetPropertyAnimationPtr myXRotate = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myShape, &Widget::setRotationX, 0, 6.28, 7000));
        myXRotate->setLoop(true);
        WidgetPropertyAnimationPtr myYRotate = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myShape, &Widget::setRotationY, 0, 6.28, 9000));
        myYRotate->setLoop(true);
        WidgetPropertyAnimationPtr myZRotate = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myShape, &Widget::setRotationZ, 0, 6.28, 13000));
        myZRotate->setLoop(true);
        animation::AnimationManager::get().play(myXRotate);
        animation::AnimationManager::get().play(myYRotate);
        animation::AnimationManager::get().play(myZRotate);

        //create a component in code
        myTransform = _mySparkWindow->getChildByName("world1")->getChildByName("transformA");
        ComponentPtr myCreated = SparkComponentFactory::get().loadSparkLayoutFromString(shared_from_this(), 
                "<Rectangle name=\"created_from_code\" width=\"300\" height=\"10\" color=\"[1.0,1.0,0.0]\"/>"); 
        myCreated->insertAtParent(boost::static_pointer_cast<spark::Container>(myTransform));

        return myBaseReturn;
        */
    }

    void DemoApp::onNextButton(EventPtr theEvent) {
        AC_PRINT << "on next button";
        _mySlides[_myCurrentSlide]->setVisible(false);
        _mySlides[_myCurrentSlide]->setSensible(false);
        _myCurrentSlide++;
        if (_myCurrentSlide >= _mySlides.size()) {
            _myCurrentSlide = 0;
        }
        AC_PRINT << ">>>>> activate slide: " << _mySlides[_myCurrentSlide]->getName();
        _mySlides[_myCurrentSlide]->setVisible(true);        
        _mySlides[_myCurrentSlide]->setSensible(true);        
    }

    void DemoApp::onTouch(EventPtr theEvent) {  
        TouchEventPtr myEvent = boost::static_pointer_cast<TouchEvent>(theEvent);
        AC_PRINT<<myEvent->getType()<<" x:"<<myEvent->getX()<<" ,y:"<<myEvent->getY();
        //
        //add two parallel animations
        ComponentPtr myTransform = _mySparkWindow->getChildByName("world1")->getChildByName("transformA");
        ComponentPtr myObject = myTransform->getChildByName("objectC");
        RectanglePtr myRectangle = boost::static_pointer_cast<spark::Rectangle>(myObject);

        WidgetPropertyAnimationPtr myAnimationC = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(myRectangle, &Widget::setX, myRectangle->getX(), myEvent->getX(), 1000,
                    animation::EasingFnc(animation::easeInOutElastic)));

        WidgetPropertyAnimationPtr myAnimationY = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(myRectangle, &Widget::setY, myRectangle->getY(), myEvent->getY(), 1000,
                    animation::EasingFnc(animation::easeInOutElastic)));
        myTransform = _mySparkWindow->getChildByName("world1")->getChildByName("transformB");
        myObject = myTransform->getChildByName("objectA");
        myRectangle = boost::static_pointer_cast<spark::Rectangle>(myObject);
        WidgetPropertyAnimationPtr myAnimationA = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myRectangle, &Widget::setRotationZ, 0, 6.28, 5000));
        
        animation::ParallelAnimationPtr myParallel = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
        myParallel->add(myAnimationC);
        myParallel->add(myAnimationY);
        myParallel->add(myAnimationA);
        animation::AnimationManager::get().play(myParallel);
    }
}


#ifdef __ANDROID__
    
/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setup(JNIEnv * env, jobject obj,  
                                                                 jlong currentMillis,
                                                                 jstring apkFile,
                                                                 jstring layoutFile);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onPause(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onResume(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onSizeChanged(JNIEnv * env, jobject obj,
                                                                 jint width, jint height);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onEvent(JNIEnv * env, jobject obj, jstring evt);
    
};

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setup(JNIEnv * env, jobject obj,  
                                                             jlong currentMillis,
                                                             jstring apkFile,
                                                             jstring layoutFile) {
    MobileSDK_Singleton::get().env = env;
    MobileSDK_Singleton::get().obj = obj;
                                                                
    jboolean isCopy;
    const char* myAssetPath = env->GetStringUTFChars(apkFile, &isCopy);
    const char* myLayoutFile = env->GetStringUTFChars(layoutFile, &isCopy);
    ourApp.setup(currentMillis, myAssetPath, myLayoutFile);
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onPause(JNIEnv * env, jobject obj) {
    ourApp.onPause();
}
JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onResume(JNIEnv * env, jobject obj) {
    ourApp.onResume();
}
JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onSizeChanged(JNIEnv * env, jobject obj,
                                                             jint width, jint height) {
    ourApp.onSizeChanged(width, height);
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onEvent(JNIEnv * env, jobject obj, jstring evt )
{
    jboolean isCopy;
    const char* myEvent = env->GetStringUTFChars(evt, &isCopy);
    ourApp.onEvent(myEvent);
}
#endif

