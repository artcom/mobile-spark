#include "DemoApp.h"

#ifdef __ANDROID__
    #include <jni.h>
#endif

#include <masl/Logger.h>
#include <animation/AnimationManager.h>
#include <animation/ParallelAnimation.h>
#include <animation/SequenceAnimation.h>
#include <animation/DelayAnimation.h>
#include <animation/Easing.h>
#include "Rectangle.h"

/////////////////// Application code, this should be in java or script language later...
namespace spark {

    DemoApp::DemoApp():BaseApp() {
    }

    DemoApp::~DemoApp() {
    }


    void freeFunction() {
        AC_PRINT << "hello from free function";
    }

    bool DemoApp::setup(const long theCurrentMillis, const std::string & theAssetPath, const std::string & theLayoutFile) {
        bool myBaseReturn = BaseApp::setup(theCurrentMillis, theAssetPath, theLayoutFile);

        //add looping sequence animation
        ComponentPtr myTransform = window->getChildByName("transformB");
        ComponentPtr myObject = myTransform->getChildByName("objectB");
        RectanglePtr myRectangle = boost::static_pointer_cast<spark::Rectangle>(myObject);
        WidgetPropertyAnimationPtr myAnimation1 = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myRectangle, &Widget::setScaleY, 0.7, 2, 500));
        WidgetPropertyAnimationPtr myAnimation2 = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myRectangle, &Widget::setScaleY, 2, 0.7, 1500, animation::easeInOutQuad));
        animation::DelayAnimationPtr myDelay = animation::DelayAnimationPtr(new animation::DelayAnimation(2000));
        animation::SequenceAnimationPtr mySequence = animation::SequenceAnimationPtr(new animation::SequenceAnimation());
        mySequence->add(myAnimation1);
        mySequence->add(myAnimation2);
        mySequence->add(myDelay);
        mySequence->setLoop(true);
        animation::AnimationManager::get().play(mySequence);

        mySequence->setOnPlay(WidgetCallbackPtr(new WidgetCallback( myRectangle, &Widget::test)));
        mySequence->setOnFinish(animation::FreeFunctionCallbackPtr(new animation::FreeFunctionCallback(freeFunction)));
        return myBaseReturn;
    }

    void DemoApp::onTouch() {

        //add two parallel animations
        ComponentPtr myTransform = window->getChildByName("transformA");
        ComponentPtr myObject = myTransform->getChildByName("objectC");
        RectanglePtr myRectangle = boost::static_pointer_cast<spark::Rectangle>(myObject);
        WidgetPropertyAnimationPtr myAnimationC = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myRectangle, &Widget::setX, 0, 5));

        myTransform = window->getChildByName("transformB");
        myObject = myTransform->getChildByName("objectA");
        myRectangle = boost::static_pointer_cast<spark::Rectangle>(myObject);
        WidgetPropertyAnimationPtr myAnimationA = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myRectangle, &Widget::setRotationZ, 0, 6.28, 5000));
        
        animation::ParallelAnimationPtr myParallel = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
        myParallel->add(myAnimationC);
        myParallel->add(myAnimationA);
        animation::AnimationManager::get().play(myParallel);
    }
}

/////////////////////////////////////////////////////////////////////////App-Instance
spark::DemoApp ourApp;

#ifdef __ANDROID__
    
/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setup(JNIEnv * env, jobject obj,  
                                                                 jlong currentMillis,
                                                                 jstring apkFile,
                                                                 jstring layoutFile);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onFrame(JNIEnv * env, jobject obj,
                                                                 jlong currentMillis);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onTouch(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setup(JNIEnv * env, jobject obj,  
                                                             jlong currentMillis,
                                                             jstring apkFile,
                                                             jstring layoutFile) {
    jboolean isCopy;
    const char* myAssetPath = env->GetStringUTFChars(apkFile, &isCopy);
    const char* myLayoutFile = env->GetStringUTFChars(layoutFile, &isCopy);
    ourApp.setup(currentMillis, myAssetPath, myLayoutFile);
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onFrame(JNIEnv * env, jobject obj,
                                                             jlong currentMillis) {
    ourApp.onFrame(currentMillis);
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onTouch(JNIEnv * env, jobject obj) {
    ourApp.onTouch();
}
#endif

