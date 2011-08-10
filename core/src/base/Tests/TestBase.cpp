#include "TestBase.h"
#include "../Logger.h"
#include "../XMLUtils.h"
#include "testLogger.h"
#include "testXMLNode.h"
#include "testMatrixStack.h"

namespace asl {

    void MyBaseTestSuite::setup() {
        UnitTestSuite::setup();
        addTest(new asl::Logger_UnitTest);
        addTest(new asl::XMLNode_UnitTest);
        addTest(new asl::MatrixStack_UnitTest);
    }    
}


#ifdef __ANDROID__
/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_TestBaseLib_test(JNIEnv * env, jobject obj);
};

JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_TestBaseLib_test(JNIEnv * env, jobject obj) {
    asl::MyBaseTestSuite mySuite("BaseTest", 0, 0);
    mySuite.run();
    return (env->NewStringUTF(mySuite.getFailedTestMessages().c_str()));
    
}
#endif


