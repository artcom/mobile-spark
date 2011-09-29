#ifndef TESTAndroid
#define TESTAndroid

#ifdef iOS
    //iOS
#elif ANDROID
    //Android
    #include <jni.h>
#endif

#include <string>
#include <masl/UnitTest.h>


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

