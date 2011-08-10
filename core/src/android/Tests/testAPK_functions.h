/* __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2008, ART+COM AG Berlin, Germany <www.artcom.de>
//
// These coded instructions, statements, and computer programs contain
// proprietary information of ART+COM AG Berlin, and are copy protected
// by law. They may be used, modified and redistributed under the terms
// of GNU General Public License referenced below.
//
// Alternative licensing without the obligations of the GPL is
// available upon request.
//
// GPL v3 Licensing:
//
// This file is part of the ART+COM Y60 Platform.
//
// ART+COM Y60 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// ART+COM Y60 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with ART+COM Y60.  If not, see <http://www.gnu.org/licenses/>.
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
*/
#ifndef _ac_mobile_asl_test_APK_functions_h_included_
#define _ac_mobile_asl_test_APK_functions_h_included_

#include <UnitTest.h>
#include "../APK_functions.h"


namespace android {
    class APK_functions_UnitTest : public UnitTest {
        public:
            APK_functions_UnitTest(JNIEnv* env, jstring apkPath) : 
                env(env), apkPath(apkPath), UnitTest("APK_functions_UnitTest") {  }

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
