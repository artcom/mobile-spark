// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "BaseEntry.h"
#include "Logger.h"
#include "string_functions.h"
#include "SingletonManager.h"
#include "MobileSDK.h"

#include "XMLUtils.h"
#include <dlfcn.h>
#include <cstdlib>

#ifdef ANDROID

#ifndef OSX
// XXX: fixes linker warning: "type and size of dynamic symbol `__dso_handle' are not defined"
extern "C"
{
    void *__dso_handle = NULL;
}
#endif
/////////////////////////////////////////////////////////////////////////JNI
JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_putEnv(JNIEnv *env, jclass, jstring envVar)
{
    const char * myEnvVar = env->GetStringUTFChars(envVar, NULL);
    putenv(strdup(myEnvVar));
    env->ReleaseStringUTFChars(envVar, myEnvVar);
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_log(JNIEnv * env, jobject obj,
                                                              jobject theSeverity, jstring theFilename,
                                                              int theLineNumber, jstring theMessage) {
    const char* myMessage = env->GetStringUTFChars(theMessage, NULL);
    const char* myFilename = env->GetStringUTFChars(theFilename, NULL);
    jclass enumClass = env->FindClass("com/artcom/mobile/Base/Severity");
    jmethodID getOrdinalMethod = env->GetMethodID(enumClass, "ordinal", "()I");
    int value = env->CallIntMethod(theSeverity, getOrdinalMethod);
    if (masl::Logger::get().ifLog((masl::Severity)value, myFilename, theLineNumber)) {
        masl::Logger::get().log(/*myTime,*/ (masl::Severity)value, myFilename, theLineNumber, myMessage);
    }
    env->ReleaseStringUTFChars(theFilename, myFilename);
    env->ReleaseStringUTFChars(theMessage, myMessage);

}
JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setLoggerTopLevelTag(JNIEnv * env, jobject obj,
                                                                         jstring theLogString) {
    const char* myLogString = env->GetStringUTFChars(theLogString, NULL);
    masl::Logger::get().setLoggerTopLevelTag(myLogString);
    env->ReleaseStringUTFChars(theLogString, myLogString);
}

JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setSeverity(JNIEnv * env, jobject obj,
                                                                         jobject theSeverity) {
    jclass enumClass = env->FindClass("com/artcom/mobile/Base/Severity");
    jmethodID getOrdinalMethod = env->GetMethodID(enumClass, "ordinal", "()I");
    int value = env->CallIntMethod(theSeverity, getOrdinalMethod);
    masl::Severity mySeverity = (masl::Severity)value;
    masl::Logger::get().setSeverity(mySeverity);
}
#endif


