#ifndef TESTBASE
#define TESTBASE

#ifdef __APPLE__
    //iOS
#elif __ANDROID__
    //Android
    #include <jni.h>
#endif

#include <string>
#include "../UnitTest.h"
#include "../Logger.h"

namespace asl {
                    
    class MyBaseTestSuite : public UnitTestSuite {
        public:
            MyBaseTestSuite(const char * myName, int argc, char *argv[]) : UnitTestSuite(myName, argc, argv) {}
            void setup();
    };
        
};

#endif //TESTBASE

