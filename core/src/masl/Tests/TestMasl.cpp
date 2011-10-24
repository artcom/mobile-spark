// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "TestMasl.h"

#include "../Logger.h"
#include "../XMLUtils.h"
#include "testLogger.h"
#include "testXMLNode.h"
#include "testMatrixStack.h"

namespace masl {

    void MyBaseTestSuite::setup() {
        UnitTestSuite::setup();
        addTest(new masl::Logger_UnitTest);
        addTest(new masl::XMLNode_UnitTest);
        addTest(new masl::MatrixStack_UnitTest);
    }
}


#ifdef ANDROID
/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_BaseBinding_test(JNIEnv * env, jobject obj);
};

JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_BaseBinding_test(JNIEnv * env, jobject obj) {
    masl::MyBaseTestSuite mySuite("BaseTest", 0, 0);
    mySuite.run();
    return (env->NewStringUTF(mySuite.getFailedTestMessages().c_str()));

}
#endif


