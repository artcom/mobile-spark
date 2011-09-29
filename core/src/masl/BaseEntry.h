#ifndef BASEENTRY
#define BASEENTRY

#ifdef iOS
    //iOS
#elif ANDROID
    //Android
    #include <jni.h>
#endif
#include <string>

#include "UnitTest.h"
#include "Logger.h"

namespace masl {
}
#ifdef ANDROID
extern "C" {
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_putEnv(JNIEnv *env, jclass, jstring envVar);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_log(JNIEnv * env, jobject obj,
                                                                  jobject theSeverity, jstring theFilename,
                                                                  int theLineNumber, jstring theMessage);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setLoggerTopLevelTag(JNIEnv * env, jobject obj,
                                                                          jstring theLogString);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setSeverity(JNIEnv * env, jobject obj,
                                                                          jobject theSeverity);

};
#endif //ANDROID

#endif //BASEENTRY

