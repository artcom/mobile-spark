#ifndef _included_mobile_android_jnibinding
#define _included_mobile_android_jnibinding

#ifdef __ANDROID__
    #include <jni.h>


#define CALL_NATIVE(THE_CALL) \
    std::string myMessage = ""; \
    jclass Exception = env->FindClass("com/artcom/mobile/Base/NativeException"); \
    try { \
        THE_CALL; \
    } catch(masl::Exception & ex) { \
        AC_ERROR << "masl::Exception caught "<< ex; \
        myMessage = ex.what() + " : " + ex.where(); \
    } catch(std::exception & ex) { \
        AC_ERROR << "std::exception caught "<< ex.what(); \
        myMessage = ex.what(); \
    } catch(...) { \
        AC_ERROR << "unknown exception caught "; \
        myMessage = "unknown exception caught"; \
    } \
    if (myMessage != "") { \
        env->ThrowNew(Exception, myMessage.c_str()); \
    }

extern "C" {
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_setup(JNIEnv * env, jobject obj,  
                                                                 jlong currentMillis,
                                                                 jstring apkFile,
                                                                 jstring layoutFile);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onPause(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onResume(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onSizeChanged(JNIEnv * env, jobject obj,
                                                                 jint width, jint height);
    JNIEXPORT void JNICALL Java_com_artcom_mobile_Base_NativeBinding_onEvent(JNIEnv * env, jobject obj, jstring evt);
    
};
#endif
#endif
