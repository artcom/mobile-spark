#include "TestAnimation.h"

#include "testPropertyAnimation.h"

namespace animation {

    void MyAnimationTestSuite::setup() {
        UnitTestSuite::setup();
        addTest(new animation::PropertyAnimation_UnitTest);
    }    
}


#ifdef __ANDROID__
/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_AnimationBinding_test(JNIEnv * env, jobject obj);
};

JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_AnimationBinding_test(JNIEnv * env, jobject obj) {
    animation::MyAnimationTestSuite mySuite("AnimationTest", 0, 0);
    mySuite.run();
    return (env->NewStringUTF(mySuite.getFailedTestMessages().c_str()));
    
}
#endif


