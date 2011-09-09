#ifdef __ANDROID__

#include "JNIBinding.h"

#include <masl/Logger.h>
#include <masl/MobileSDK_Singleton.h>

#include "BaseApp.h"

extern spark::BaseAppPtr ourApp;

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setup(JNIEnv * env, jobject obj,  
                                                             jlong currentMillis,
                                                             jstring apkFile,
                                                             jstring layoutFile) {
    MobileSDK_Singleton::get().env = env;
    MobileSDK_Singleton::get().obj = obj;
                                                                
    jboolean isCopy;
    const char* myAssetPath = env->GetStringUTFChars(apkFile, &isCopy);
    const char* myLayoutFile = env->GetStringUTFChars(layoutFile, &isCopy);
    CALL_NATIVE(ourApp->setup(currentMillis, myAssetPath, myLayoutFile));
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onPause(JNIEnv * env, jobject obj) {
    CALL_NATIVE(ourApp->onPause());
}
JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onResume(JNIEnv * env, jobject obj) {
    CALL_NATIVE(ourApp->onResume());
}
JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onSizeChanged(JNIEnv * env, jobject obj,
                                                             jint width, jint height) {
    CALL_NATIVE(ourApp->onSizeChanged(width, height));
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onEvent(JNIEnv * env, jobject obj, jstring evt )
{
    jboolean isCopy;
    const char* myEvent = env->GetStringUTFChars(evt, &isCopy);
    CALL_NATIVE(ourApp->onEvent(myEvent));
}

#endif