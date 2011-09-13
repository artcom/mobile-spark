#ifdef __ANDROID__

#include "JNIBinding.h"

#include <masl/Logger.h>
#include <masl/MobileSDK.h>
#include <android/AndroidMobileSDK.h>
#include "BaseApp.h"

extern spark::BaseAppPtr ourApp;

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setup(JNIEnv * env, jobject obj,  
                                                             jlong currentMillis,
                                                             jstring apkFile,
                                                             jstring layoutFile) {
                                                                
    jboolean isCopy;
    const char* myAssetPath = env->GetStringUTFChars(apkFile, &isCopy);
    const char* myLayoutFile = env->GetStringUTFChars(layoutFile, &isCopy);
    CALL_NATIVE(ourApp->setup(currentMillis, myAssetPath, myLayoutFile));
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_initBinding(JNIEnv * env, jobject obj) {
    android::AndroidMobileSDKPtr myAndroidSDK = boost::static_pointer_cast<android::AndroidMobileSDK>(MobileSDK_Singleton::get().getNative());

    myAndroidSDK->env = env;
    myAndroidSDK->obj = obj;
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
