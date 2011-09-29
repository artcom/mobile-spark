#ifndef TESTBASE
#define TESTBASE

#ifdef iOS
    //iOS
#elif ANDROID
    //Android
    #include <jni.h>
#endif

#include <string>
#include "../UnitTest.h"

namespace masl {

    class MyBaseTestSuite : public UnitTestSuite {
        public:
            MyBaseTestSuite(const char * myName, int argc, char *argv[]) : UnitTestSuite(myName, argc, argv) {}
            void setup();
    };

};

#endif //TESTBASE

