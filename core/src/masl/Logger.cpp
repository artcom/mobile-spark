#include "Settings.h"

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
    Logger::Logger() : _myTopLevelLogTag("Unset"), _myGlobalSeverity(SEV_WARNING) {}
    Logger::~Logger() {}


    void Logger::setLoggerTopLevelTag(const std::string & theTagString) {
        _myTopLevelLogTag = theTagString;
    }
    void Logger::log(/*Time theTime, */ Severity theSeverity, const char * theModule, int theId, const std::string & theText) {
        char buf[20];
        sprintf(buf,"%i",theId);
        std::string myLogTag(_myTopLevelLogTag);
        myLogTag += "/";
        if (theSeverity == SEV_TESTRESULT) {
            myLogTag += "TestResult";
            myLogTag += "/";
        }
        myLogTag += removeExtension(lastFileNamePart(theModule));
        std::string myText(theText);
        myText += " [";
        myText += lastFileNamePart(theModule);
        myText += " at:" + std::string(buf) + "]";


        #ifdef __APPLE__
            //iOS
        switch (theSeverity) {
            case SEV_TRACE:
                std::cout << myLogTag.c_str() << " TRACE: " << myText.c_str() << "\n";
                break;
            case SEV_DEBUG:
                std::cout << myLogTag.c_str() << " DEBUG: " << myText.c_str() << "\n";
                break;
            case SEV_INFO:
                std::cout << myLogTag.c_str() << " INFO: " << myText.c_str() << "\n";
                break;
            case SEV_WARNING:
                std::cout << myLogTag.c_str() << " WARNING: " << myText.c_str() << "\n";
                break;
            case SEV_PRINT:
                std::cout << myLogTag.c_str() << " LOG: " << myText.c_str() << "\n";
                break;
            case SEV_ERROR:
                std::cout << myLogTag.c_str() << " ERROR: " << myText.c_str() << "\n";
                break;
            default:
                throw Exception("Unknown logger severity");
                break;
        }

        #elif __ANDROID__
            //Android
        switch (theSeverity) {
            case SEV_TRACE :
                __android_log_print(ANDROID_LOG_VERBOSE, myLogTag.c_str(), myText.c_str());//__VA_ARGS__)
                break;
            case SEV_DEBUG :
                __android_log_print(ANDROID_LOG_DEBUG, myLogTag.c_str(), myText.c_str());//__VA_ARGS__)
                break;
            case SEV_WARNING :
                __android_log_print(ANDROID_LOG_WARN, myLogTag.c_str(), myText.c_str());//__VA_ARGS__)
                break;
            case SEV_INFO :
            case SEV_PRINT :
                __android_log_print(ANDROID_LOG_INFO, myLogTag.c_str(), myText.c_str());//__VA_ARGS__)
                break;
            case SEV_ERROR :
                __android_log_print(ANDROID_LOG_ERROR, myLogTag.c_str(), myText.c_str());//__VA_ARGS__)
                break;
            case SEV_TESTRESULT :
                __android_log_print(ANDROID_LOG_INFO, myLogTag.c_str(), myText.c_str());//__VA_ARGS__)
                break;
            default:
                throw Exception("Unknown logger severity");
                break;
        }
        #endif

    }

};
