#include "BaseEntry.h"
#include "Logger.h"
#include "XMLUtils.h"
#include "testLogger.h"
#include "testXMLNode.h"
#include "testMatrixStack.h"

namespace asl {

    bool loadSpark(const std::string & theFilename) {
        AC_PRINT << "load file: " << theFilename.c_str();
        //test xml-loading
        xmlNode* rootNode = loadXML(theFilename);
        AC_PRINT << "this is cpp land"; 
        return true;
    }    
}


#ifdef __ANDROID__
/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT bool JNICALL Java_com_artcom_mobile_BaseNativeLib_loadSpark(JNIEnv * env, jobject obj,
                                                                          jstring filename);
};

JNIEXPORT bool JNICALL Java_com_artcom_mobile_BaseNativeLib_loadSpark(JNIEnv * env, jobject obj,
                                                                 jstring filename) {
    jboolean isCopy;
    const char* str = env->GetStringUTFChars(filename, &isCopy);                                                                    
    return asl::loadSpark(str);    
}
#endif


