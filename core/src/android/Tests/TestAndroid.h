// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef TESTAndroid
#define TESTAndroid

#ifdef iOS
    //iOS
#elif ANDROID
    //Android
    #include <jni.h>
#endif

#include <string>
#include <masl/UnitTest.h>


namespace android {

    class MyAndroidTestSuite : public UnitTestSuite {
        public:
            MyAndroidTestSuite(const char * myName, int argc, char *argv[]) : UnitTestSuite(myName, argc, argv) {}
            void setup();
            JNIEnv *env;
            jstring apkPath;
    };

};

#endif

