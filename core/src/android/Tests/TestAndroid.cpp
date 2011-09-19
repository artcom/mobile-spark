#include "TestAndroid.h"

#include <masl/Logger.h>
#include "testAPK_functions.h"


namespace android {
    void MyAndroidTestSuite::setup() {
        UnitTestSuite::setup();
        addTest(new android::APK_functions_UnitTest(env, apkPath));
    }
}


#ifdef __ANDROID__
/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_AndroidBinding_test(JNIEnv * env, jobject obj, jstring apkPath);
};
JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_AndroidBinding_test(JNIEnv * env, jobject obj, jstring apkPath) {
    android::MyAndroidTestSuite mySuite("AndroidTest", 0, 0);
    mySuite.env = env;
    mySuite.apkPath = apkPath;
    mySuite.run();
    return (env->NewStringUTF(mySuite.getFailedTestMessages().c_str()));
}
#endif


