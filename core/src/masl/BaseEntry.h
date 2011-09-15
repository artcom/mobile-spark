#ifndef BASEENTRY
#define BASEENTRY

#ifdef __APPLE__
    //iOS
#elif __ANDROID__
    //Android
    #include <jni.h>
#endif
#include <string>

#include "UnitTest.h"
#include "Logger.h"

namespace masl {    
}
#ifdef __ANDROID__
extern "C" {
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_initBinding(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_putEnv(JNIEnv *env, jclass, jstring envVar);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_log(JNIEnv * env, jobject obj,
                                                                  jobject theSeverity, jstring theFilename, 
                                                                  int theLineNumber, jstring theMessage);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setLoggerTopLevelTag(JNIEnv * env, jobject obj,
                                                                          jstring theLogString);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setSeverity(JNIEnv * env, jobject obj,
                                                                          jobject theSeverity);
                                                                          
};
#endif //__ANDROID__

#endif //BASEENTRY

