#ifndef TESTSPARK
#define TESTSPARK

#ifdef __APPLE__
    //iOS
#elif __ANDROID__
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

