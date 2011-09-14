#ifdef __ANDROID__

#include "JNIBinding.h"

#include <masl/Logger.h>

#include "AppProvider.h"
    
JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setup(JNIEnv * env, jobject obj,  
                                                             jlong currentMillis,
                                                             jstring apkFile) {
    const char* myAssetPath = env->GetStringUTFChars(apkFile, NULL);
    CALL_NATIVE(spark::AppProvider::get().getApp()->setup(currentMillis, myAssetPath));
    env->ReleaseStringUTFChars(apkFile, myAssetPath);
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
