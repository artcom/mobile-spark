#ifdef __ANDROID__

#include "JNIBinding.h"

#include <masl/Logger.h>
#include <masl/MobileSDK_Singleton.h>

#include "AppProvider.h"

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setup(JNIEnv * env, jobject obj,  
                                                             jlong currentMillis,
                                                             jstring apkFile,
                                                             jint theScreenWidth,
                                                             jint theScreenHeight) {
    jboolean isCopy;
    const char* myAssetPath = env->GetStringUTFChars(apkFile, &isCopy);
    CALL_NATIVE(spark::AppProvider::get().getApp()->setup(currentMillis, myAssetPath, theScreenWidth, theScreenHeight));
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_initBinding(JNIEnv * env, jobject obj) {
    MobileSDK_Singleton::get().env = env;
    MobileSDK_Singleton::get().obj = obj;
}
JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onResume(JNIEnv * env, jobject obj) {
    CALL_NATIVE(spark::AppProvider::get().getApp()->onResume());
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onEvent(JNIEnv * env, jobject obj, jstring evt )
{
    jboolean isCopy;
    const char* myEvent = env->GetStringUTFChars(evt, &isCopy);
    CALL_NATIVE(spark::AppProvider::get().getApp()->onEvent(myEvent));
}

#endif
