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

namespace masl {    
};

#endif //BASEENTRY

