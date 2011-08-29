#include "BaseEntry.h"
#include "Logger.h"
#include "string_functions.h"
#include "SingletonManager.h"

#include "XMLUtils.h"
#include <dlfcn.h>

namespace masl {
    static bool loadSpark(const std::string & theFilename) {
        AC_PRINT << "load file: " << theFilename.c_str();
        //test xml-loading
        xmlDocPtr doc = loadXML(theFilename);
        xmlFreeDoc(doc);
        AC_PRINT << "this is cpp land"; 
        return true;
    }    
}


#ifdef __ANDROID__

// XXX: fixes linker warning: "type and size of dynamic symbol `__dso_handle' are not defined"
extern "C"
{
    void *__dso_handle = NULL;
}
/////////////////////////////////////////////////////////////////////////JNI
extern "C" {
    JNIEXPORT bool JNICALL Java_com_artcom_mobile_Base_NativeBinding_loadSpark(JNIEnv * env, jobject obj,
                                                                          jstring filename);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_log(JNIEnv * env, jobject obj,
                                                                  int theSeverity, jstring theFilename, 
                                                                  int theLineNumber, jstring theMessage);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setLoggerTopLevelTag(JNIEnv * env, jobject obj,
                                                                          jstring theLogString);
                                                                          
                                                                          
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
JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setLoggerTopLevelTag(JNIEnv * env, jobject obj,
                                                                         jstring theLogString) {
    jboolean isCopy;
    const char* myLogString = env->GetStringUTFChars(theLogString, &isCopy);                                                                    
    masl::Logger::get().setLoggerTopLevelTag(myLogString);
}

#endif


