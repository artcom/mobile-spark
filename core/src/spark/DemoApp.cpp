#include "DemoApp.h"

#include <jni.h>

#include <animation/AnimationManager.h>


/////////////////// Application code, this should be in java or script language later...
namespace spark {
    DemoApp::DemoApp():BaseApp() {
    }
    DemoApp::~DemoApp() {
    }

    void DemoApp::onTouch() {
        animation::AnimationPtr myAnimation = animation::AnimationPtr(new animation::Animation());
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
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onFrame(JNIEnv * env, jobject obj);
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

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onFrame(JNIEnv * env, jobject obj) {
    ourApp.onFrame();
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onTouch(JNIEnv * env, jobject obj) {
    ourApp.onTouch();
}
#endif

