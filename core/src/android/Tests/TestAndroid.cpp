// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "TestAndroid.h"

#include <masl/Logger.h>
#include "testAPK_functions.h"


namespace android {
    void MyAndroidTestSuite::setup() {
        UnitTestSuite::setup();
        addTest(new android::APK_functions_UnitTest(env, apkPath));
    }
}


#ifdef ANDROID
/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_AndroidBinding_test(JNIEnv * env, jobject obj, jstring apkPath);
};
JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_AndroidBinding_test(JNIEnv * env, jobject obj, jstring apkPath) {
    android::MyAndroidTestSuite mySuite("AndroidTest", 0, 0);
    mySuite.env = env;
    mySuite.apkPath = apkPath;
    mySuite.run();
    return (env->NewStringUTF(mySuite.getFailedTestMessages().c_str()));
}
#endif


