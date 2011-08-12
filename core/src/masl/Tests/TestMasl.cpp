#include "TestMasl.h"

#include "../Logger.h"
#include "../XMLUtils.h"
#include "testLogger.h"
#include "testXMLNode.h"
#include "testMatrixStack.h"

namespace masl {

    void MyBaseTestSuite::setup() {
        UnitTestSuite::setup();
        addTest(new masl::Logger_UnitTest);
        addTest(new masl::XMLNode_UnitTest);
        addTest(new masl::MatrixStack_UnitTest);
    }    
}


#ifdef __ANDROID__
/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_TestBaseBinding_test(JNIEnv * env, jobject obj);
};

<<<<<<< HEAD:core/src/base/Tests/TestBase.cpp
JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_TestBaseBinding_test(JNIEnv * env, jobject obj) {
    asl::MyBaseTestSuite mySuite("BaseTest", 0, 0);
=======
JNIEXPORT jstring JNICALL Java_com_artcom_mobile_test_TestBaseLib_test(JNIEnv * env, jobject obj) {
    masl::MyBaseTestSuite mySuite("BaseTest", 0, 0);
>>>>>>> c2e9dd77ce181ac944dd3fa38ec69ec0a3419a99:core/src/masl/Tests/TestMasl.cpp
    mySuite.run();
    return (env->NewStringUTF(mySuite.getFailedTestMessages().c_str()));
    
}
#endif


