#ifndef TESTSPARK
#define TESTSPARK

#ifdef iOS
    //iOS
#elif ANDROID
    //Android
    #include <jni.h>
#endif

#include <string>
#include <masl/UnitTest.h>

namespace spark {

    class MySparkTestSuite : public UnitTestSuite {
        public:
            MySparkTestSuite(const char * myName, int argc, char *argv[]) : UnitTestSuite(myName, argc, argv) {}
            void setup();
    };

};

#endif //TESTSPARK

