#ifndef TESTAndroid
#define TESTAndroid

#ifdef __APPLE__
    //iOS
#elif __ANDROID__
    //Android
    #include <jni.h>
#endif

#include <string>
#include <masl/UnitTest.h>

using namespace masl;

namespace android {
                    
    class MyAndroidTestSuite : public UnitTestSuite {
        public:
            MyAndroidTestSuite(const char * myName, int argc, char *argv[]) : UnitTestSuite(myName, argc, argv) {}
            void setup();
            JNIEnv *env;
            jstring apkPath;
    };
        
};

#endif 

