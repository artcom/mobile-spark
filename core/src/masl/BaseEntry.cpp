#include "BaseEntry.h"
#include "Logger.h"
#include "XMLUtils.h"

namespace masl {

    bool loadSpark(const std::string & theFilename) {
        AC_PRINT << "load file: " << theFilename.c_str();
        //test xml-loading
        xmlDocPtr doc = loadXML(theFilename);
        xmlFreeDoc(doc);
        AC_PRINT << "this is cpp land"; 
        return true;
    }    
}


#ifdef __ANDROID__
/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT bool JNICALL Java_com_artcom_mobile_Base_NativeBinding_loadSpark(JNIEnv * env, jobject obj,
                                                                          jstring filename);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_log(JNIEnv * env, jobject obj,
                                                                  int theSeverity, jstring theFilename, 
                                                                  int theLineNumber, jstring theMessage);
                                                                          
};

JNIEXPORT bool JNICALL Java_com_artcom_mobile_Base_NativeBinding_loadSpark(JNIEnv * env, jobject obj,
                                                                 jstring filename) {
    jboolean isCopy;
    const char* str = env->GetStringUTFChars(filename, &isCopy);                                                                    
    return masl::loadSpark(str);    
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_log(JNIEnv * env, jobject obj,
                                                              int theSeverity, jstring theFilename, 
                                                              int theLineNumber, jstring theMessage) {
    jboolean isCopy;
    const char* myMessage = env->GetStringUTFChars(theMessage, &isCopy);                                                                    
    const char* myFilename = env->GetStringUTFChars(theFilename, &isCopy);                                                                    
    masl::Logger::get().log(/*myTime,*/ masl::SEV_PRINT, myFilename, theLineNumber, myMessage);
}

#endif


