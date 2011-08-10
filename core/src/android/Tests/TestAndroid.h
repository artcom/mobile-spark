#ifndef TESTAndroid
#define TESTAndroid

#ifdef __APPLE__
    //iOS
#elif __ANDROID__
    //Android
    #include <jni.h>
#endif

#include <string>
#include <UnitTest.h>

using namespace asl;

namespace android {
                    
    class MyAndroidTestSuite : public UnitTestSuite {
        public:
            MyAndroidTestSuite(const char * myName, int argc, char *argv[]) : UnitTestSuite(myName, argc, argv) {}
            void setup();
    };
        
};

#endif 

