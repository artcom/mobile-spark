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

#include "Logger.h"
#include "Exception.h"
#include "string_functions.h"
#ifdef __APPLE__
    //iOS
    #include <iostream>
#elif __ANDROID__
    //Android
    #include <android/log.h>
#endif

namespace masl {
    Logger::Logger() {}
        
        
    void Logger::setLoggerTopLevelTag(const std::string & theTagString) {
        _myTopLevelLogTag = theTagString;
    }
    void Logger::log(/*Time theTime, */ Severity theSeverity, const char * theModule, int theId, const std::string & theText) {
        char buf[20];
        sprintf(buf,"%i",theId);
        std::string myLogTag("/");
        myLogTag += _myTopLevelLogTag;
        myLogTag += "/";
        if (theSeverity == SEV_TESTRESULT) {
            myLogTag += "TestResult";
            myLogTag += "/";
        }
        myLogTag += "[";
        myLogTag += lastFileNamePart(theModule);
        myLogTag += " at:" + std::string(buf) + "]";
            
                
        #ifdef __APPLE__
            //iOS
        switch (theSeverity) {
            case SEV_WARNING:
                std::cout << myLogTag.c_str() << " WARNING: " << theText.c_str() << "\n";
                break;
            case SEV_PRINT:
                std::cout << myLogTag.c_str() << " LOG: " << theText.c_str() << "\n";
                break;
            case SEV_ERROR:
                std::cout << myLogTag.c_str() << " ERROR: " << theText.c_str() << "\n";
                break;
            default:
                throw Exception("Unknown logger severity");
                break;
        }
            
        #elif __ANDROID__
            //Android
        switch (theSeverity) {
            case SEV_WARNING :
                __android_log_print(ANDROID_LOG_WARN, myLogTag.c_str(), theText.c_str());//__VA_ARGS__) 
                break;
            case SEV_PRINT :
                __android_log_print(ANDROID_LOG_INFO, myLogTag.c_str(), theText.c_str());//__VA_ARGS__) 
                break;
            case SEV_ERROR :
                __android_log_print(ANDROID_LOG_ERROR, myLogTag.c_str(), theText.c_str());//__VA_ARGS__) 
                break;
            case SEV_TESTRESULT :
                __android_log_print(ANDROID_LOG_INFO, myLogTag.c_str(), theText.c_str());//__VA_ARGS__) 
                break;
            default:
                //throw Exception("Unknown logger severity");
                __android_log_print(ANDROID_LOG_INFO, myLogTag.c_str(), theText.c_str());//__VA_ARGS__) 
                break;
        }
        #endif
               
    }
   
};
