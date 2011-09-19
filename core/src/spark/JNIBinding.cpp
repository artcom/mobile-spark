#ifdef __ANDROID__

#include "JNIBinding.h"

#include <masl/Logger.h>

#include "AppProvider.h"
#include <masl/MobileSDK.h>
#include <android/AndroidMobileSDK.h>

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setup(JNIEnv * env, jobject obj,
                                                             jlong currentMillis,
                                                             jstring apkFile,
                                                             jint theScreenWidth,
                                                             jint theScreenHeight) {
    jboolean isCopy;
    const char* myAssetPath = env->GetStringUTFChars(apkFile, &isCopy);
    CALL_NATIVE(spark::AppProvider::get().getApp()->setup(currentMillis, myAssetPath, theScreenWidth, theScreenHeight));
    env->ReleaseStringUTFChars(apkFile, myAssetPath);
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_initBinding(JNIEnv * env, jobject obj) {
    android::AndroidMobileSDKPtr myAndroidSDK = android::AndroidMobileSDKPtr(new android::AndroidMobileSDK());
    MobileSDK_Singleton::get().setMobileSDK(myAndroidSDK);

    myAndroidSDK->env = env;
    myAndroidSDK->obj = obj;
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onResume(JNIEnv * env, jobject obj) {
    CALL_NATIVE(spark::AppProvider::get().getApp()->onResume());
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onEvent(JNIEnv * env, jobject obj, jstring evt )
{
    const char* myEvent = env->GetStringUTFChars(evt, NULL);
    CALL_NATIVE(spark::AppProvider::get().getApp()->onEvent(myEvent));
    env->ReleaseStringUTFChars(evt, myEvent);
}

#endif
