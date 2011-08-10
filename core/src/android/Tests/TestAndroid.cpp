#include "TestAndroid.h"
#include <Logger.h>
#include "testAPK_functions.h"


namespace android {
    void MyAndroidTestSuite::setup() {
        UnitTestSuite::setup();
        addTest(new android::APK_functions_UnitTest);
    }    
}


#ifdef __ANDROID__
/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_TestAndroidLib_test(JNIEnv * env, jobject obj);
};

JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_TestAndroidLib_test(JNIEnv * env, jobject obj) {
    android::MyAndroidTestSuite mySuite("AndroidTest", 0, 0);
    mySuite.run();
    return (env->NewStringUTF(mySuite.getFailedTestMessages().c_str()));
}
#endif


