#include "DemoApp.h"

#include <jni.h>

#include <animation/PropertyAnimation.h>
#include <animation/AnimationManager.h>
#include "Rectangle.h"


/////////////////// Application code, this should be in java or script language later...
namespace spark {

    DemoApp::DemoApp():BaseApp() {
    }
    DemoApp::~DemoApp() {
    }

    void DemoApp::onTouch() {
        ComponentPtr myTransform = window->getChildByName("transformA");
        ComponentPtr myObject = myTransform->getChildByName("objectC");
        RectanglePtr myCRectangle = boost::static_pointer_cast<spark::Rectangle>(myObject);
        animation::PropertyAnimationPtr myAnimation = animation::PropertyAnimationPtr(new animation::PropertyAnimation(setTest));
        animation::AnimationManager::get().play(myAnimation); 
    }
}

/////////////////////////////////////////////////////////////////////////App-Instance
spark::DemoApp ourApp;

#ifdef __ANDROID__
    
/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setup(JNIEnv * env, jobject obj,  
                                                                 jstring apkFile,
                                                                 jstring layoutFile);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onFrame(JNIEnv * env, jobject obj,
                                                                 jlong currentMillis);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onTouch(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setup(JNIEnv * env, jobject obj,  
                                                             jstring apkFile,
                                                             jstring layoutFile) {
    jboolean isCopy;
    const char* myAssetPath = env->GetStringUTFChars(apkFile, &isCopy);
    const char* myLayoutFile = env->GetStringUTFChars(layoutFile, &isCopy);
    ourApp.setup(myAssetPath, myLayoutFile);
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onFrame(JNIEnv * env, jobject obj,
                                                             jlong currentMillis) {
    ourApp.onFrame(currentMillis);
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onTouch(JNIEnv * env, jobject obj) {
    ourApp.onTouch();
}
#endif

