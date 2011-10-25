// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "TestSpark.h"

#include "testSparkEvents.h"

namespace spark {

    void MySparkTestSuite::setup() {
        UnitTestSuite::setup();
        addTest(new spark::SparkEvents_UnitTest);
    }
}


#ifdef ANDROID
/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_SparkBinding_test(JNIEnv * env, jobject obj);
};

JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_SparkBinding_test(JNIEnv * env, jobject obj) {
    spark::MySparkTestSuite mySuite("SparkTest", 0, 0);
    mySuite.run();
    return (env->NewStringUTF(mySuite.getFailedTestMessages().c_str()));

}
#endif


