// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifdef ANDROID

#include "JNIBinding.h"

#include <masl/Exception.h>
#include <masl/Logger.h>
#include <masl/MobileSDK.h>
#include <android/AndroidMobileSDK.h>

#include "AppProvider.h"

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setup(JNIEnv * env, jobject obj,
                                                             jlong currentMillis,
                                                             jstring apkFile,
                                                             jint theScreenWidth,
                                                             jint theScreenHeight) {
    jboolean isCopy;
    const char* myAssetPath = env->GetStringUTFChars(apkFile, &isCopy);
    if (!spark::AppProvider::get().getApp()->isSetup()) {
        CALL_NATIVE(spark::AppProvider::get().getApp()->setup(currentMillis, myAssetPath, theScreenWidth, theScreenHeight));        
    } else {
        // a second call to setup is really a resume
        CALL_NATIVE(spark::AppProvider::get().getApp()->onResume());        
    }
    env->ReleaseStringUTFChars(apkFile, myAssetPath);
}


JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_initBinding(JNIEnv * env, jobject obj) {
    android::AndroidMobileSDKPtr myAndroidSDK = android::AndroidMobileSDKPtr(new android::AndroidMobileSDK());
    masl::MobileSDK_Singleton::get().setMobileSDK(myAndroidSDK);

    myAndroidSDK->env = env;
    myAndroidSDK->obj = obj;
}


JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onPause(JNIEnv * env, jobject obj) {
    CALL_NATIVE(spark::AppProvider::get().getApp()->onPause());
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

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_handleEvents(JNIEnv * env, jobject obj, jstring evt) {
    CALL_NATIVE(spark::AppProvider::get().getApp()->handleEvents());    
}


#endif
