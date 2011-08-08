#include "CppEntry.h"
#include "Logger.h"
#include "testLogger.h"
#include "XMLUtils.h"
//#include <libxml/tree.h>

namespace cppcore {

    CppEntry::CppEntry() {
    }
    CppEntry::~CppEntry() {
        //TODO!!!
    }

    void CppEntry::loadXML(const std::string & theFilename) {
        AC_PRINT << "load file: " << theFilename.c_str();
        loadLayout(theFilename);
    }

    bool CppEntry::loadSpark(const std::string & theFilename) {
        //test xml-loading
        loadXML(theFilename);
        AC_PRINT << "this is cpp land"; 
        return true;
    }
    bool CppEntry::test() {
        MyTestSuite mySuite("CppEntryTest", 0, 0);
        mySuite.run();
        return mySuite.returnStatus() == 0;
    }
    
    void MyTestSuite::setup() {
        UnitTestSuite::setup();
        addTest(new asl::Logger_UnitTest);
            
    }
}


/////////////////////////////////////////////////////////////////////////App-Instance
cppcore::CppEntry myCppEntry;

#ifdef __ANDROID__
/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT bool JNICALL Java_com_artcom_mobile_BaseNativeLib_loadSpark(JNIEnv * env, jobject obj,
                                                                          jstring filename);
    JNIEXPORT bool JNICALL Java_com_artcom_mobile_BaseNativeLib_test(JNIEnv * env, jobject obj);
};

JNIEXPORT bool JNICALL Java_com_artcom_mobile_BaseNativeLib_loadSpark(JNIEnv * env, jobject obj,
                                                                 jstring filename) {
    jboolean isCopy;
    const char* str = env->GetStringUTFChars(filename, &isCopy);                                                                    
    return myCppEntry.loadSpark(str);    
}
JNIEXPORT bool JNICALL Java_com_artcom_mobile_BaseNativeLib_test(JNIEnv * env, jobject obj) {
    return myCppEntry.test();
    
}
#endif


