#include "TestSpark.h"

#include "testSparkEvents.h"

namespace spark {

    void MySparkTestSuite::setup() {
        UnitTestSuite::setup();
        addTest(new spark::SparkEvents_UnitTest);
    }    
}


#ifdef __ANDROID__
/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_SparkBinding_test(JNIEnv * env, jobject obj);
};

JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_SparkBinding_test(JNIEnv * env, jobject obj) {
    spark::MySparkTestSuite mySuite("SparkTest", 0, 0);
    mySuite.run();
    return (env->NewStringUTF(mySuite.getFailedTestMessages().c_str()));
    
}
#endif


