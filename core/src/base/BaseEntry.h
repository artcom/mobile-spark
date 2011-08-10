#ifndef BASEENTRY
#define BASEENTRY

#ifdef __APPLE__
    //iOS
#elif __ANDROID__
    //Android
    #include <jni.h>
#endif
#include <string>

#include "UnitTest.h"
#include "Logger.h"

namespace asl {    
    static bool loadSpark(const std::string & theFilename);                        
};

#endif //CPPENTRY

