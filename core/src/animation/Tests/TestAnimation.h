#ifndef TESTANIMATION
#define TESTANIMATION

#ifdef iOS
    //iOS
#elif ANDROID
    //Android
    #include <jni.h>
#endif

#include <string>
#include <masl/UnitTest.h>

namespace animation {

    class MyAnimationTestSuite : public UnitTestSuite {
        public:
            MyAnimationTestSuite(const char * myName, int argc, char *argv[]) : UnitTestSuite(myName, argc, argv) {}
            void setup();
    };

};

#endif //TESTANIMATION

