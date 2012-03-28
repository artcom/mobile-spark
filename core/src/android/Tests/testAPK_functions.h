// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_asl_test_APK_functions_h_included_
#define _ac_mobile_asl_test_APK_functions_h_included_

#include <masl/UnitTest.h>
#include <masl/AssetProvider.h>

#include "../AndroidAssetProvider.h"
#include "../APK_functions.h"


namespace android {
    class APK_functions_UnitTest : public UnitTest {
        public:
            APK_functions_UnitTest(JNIEnv* env, jstring apkPath) :
                UnitTest("APK_functions_UnitTest"), env(env), apkPath(apkPath) {  

                const char *apkString = env->GetStringUTFChars(apkPath, NULL);
                masl::AssetProviderSingleton::get().setAssetProvider(AndroidAssetProviderPtr(new AndroidAssetProvider(apkString,"")));
            }

            JNIEnv* env;
            jstring apkPath;

            void run() {
                perform_APK_functionsTest();
            }

            void perform_APK_functionsTest() {
                jboolean isCopy;
                const char* str = env->GetStringUTFChars(apkPath, &isCopy);
                ENSURE_MSG(str, "apkPath given form java should not be null");
                zip* myAPKArchive = NULL;
                loadAPK(&myAPKArchive, str);
                ENSURE_MSG(myAPKArchive, "apkArchive should not be null");
                std::string myTextFromFile = readFromPackage(myAPKArchive, "assets/test.txt");
                ENSURE_MSG(!myTextFromFile.empty(), "there should be text in file");
                ENSURE_MSG(myTextFromFile.find("foo bar") == 0, "text in file should start with 'foo bar'");
                std::vector<std::string> myLinesInFile = readLineByLineFromPackage(myAPKArchive, "assets/test.txt");
                ENSURE_MSG(myLinesInFile.size() == 2, "there should be two lines in file");
                ENSURE_MSG(myLinesInFile[1].find("baz") == 0, "second line should start with 'baz'");
            }
    };

};
#endif
