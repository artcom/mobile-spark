#include "AndroidMobileSDK.h"
#include <masl/Logger.h>

namespace android {
    AndroidMobileSDK::AndroidMobileSDK() {

    }

    AndroidMobileSDK::~AndroidMobileSDK() {

    }
    void AndroidMobileSDK::vibrate(long theDurationMillisec) {
        if (env) {
            jclass cls = env->FindClass("com/artcom/mobile/Base/NativeBinding");
            jmethodID myMethodId = env->GetStaticMethodID(cls, "vibrate", "(I)V");
            if(myMethodId != 0) {
                jvalue myArgs[1];
                myArgs[0].i =theDurationMillisec ;
                env->CallStaticVoidMethodA (cls, myMethodId, myArgs);
            } else {
                AC_WARNING  << "Sorry, java-vibrate not found";
            }
        }       
    }
    masl::TextInfo AndroidMobileSDK::renderText(const std::string & theMessage, int theTextureId, int theFontSize,
                                                vector4 theColor, int theMaxWidth, int theMaxHeight, const std::string & theAlign,
                                                const std::string & theFontPath) {
        masl::TextInfo myTextInfo;
        if (env) {
            jclass cls = env->FindClass("com/artcom/mobile/Base/NativeBinding");
            jmethodID myMethodId = env->GetStaticMethodID(cls, "renderText", "(Ljava/lang/String;II[IIILjava/lang/String;Ljava/lang/String;)Ljava/util/List;");
            if(myMethodId != 0) {
                jvalue myArgs[8];
                myArgs[0].l =  env->NewStringUTF(theMessage.c_str());
                myArgs[1].i = theTextureId;
                myArgs[2].i = theFontSize;
                jintArray jI = env->NewIntArray(4);
                jint array[] = { theColor[0] * 255, theColor[1] * 255, theColor[2] * 255, theColor[3] * 255};
                env->SetIntArrayRegion(jI, 0 , 4, array);
                myArgs[3].l = jI;
                myArgs[4].i = theMaxWidth;
                myArgs[5].i = theMaxHeight;
                myArgs[6].l = env->NewStringUTF(theAlign.c_str());;
                myArgs[7].l =  env->NewStringUTF(theFontPath.c_str());
                jobject myList = env->CallStaticObjectMethodA (cls, myMethodId, myArgs);
                jclass listClass = env->GetObjectClass(myList);
                jmethodID getMethod = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");

                jobject myInt = (jobject)env->CallObjectMethod(myList, getMethod, 0);
                jclass myIntegerClass = env->GetObjectClass(myInt);

                jmethodID intValueMethod = env->GetMethodID(myIntegerClass, "intValue", "()I");
                myTextInfo.textureID = (jint)env->CallIntMethod(myInt, intValueMethod, 0);

                myInt = (jobject)env->CallObjectMethod(myList, getMethod, 1);
                myTextInfo.width = (jint)env->CallIntMethod(myInt, intValueMethod, 0);

                myInt = (jobject)env->CallObjectMethod(myList, getMethod, 2);
                myTextInfo.height = (jint)env->CallIntMethod(myInt, intValueMethod, 0);

            } else {
                AC_WARNING  << "Sorry, java-rendertext not found";
            }
        }
        return myTextInfo;
    }

    void AndroidMobileSDK::freezeMobileOrientation(std::string theOrientation) {
        if (env) {
            jclass cls = env->FindClass("com/artcom/mobile/Base/NativeBinding");
            jmethodID myMethodId = env->GetStaticMethodID(cls, "freezeOrientation", "(Ljava/lang/String;)V");
            if(myMethodId != 0) {
                jvalue myArgs[1];
                myArgs[0].l =  env->NewStringUTF(theOrientation.c_str());
                env->CallStaticVoidMethodA (cls, myMethodId, myArgs);
            } else {
                AC_WARNING  << "Sorry, java-freezeOrientation not found";
            }
        }
    }

    masl::CameraInfo AndroidMobileSDK::getCameraSpec() {
        masl::CameraInfo myCameraInfo;
        myCameraInfo.textureID=0;
        if (env) {
            jclass cls = env->FindClass("com/artcom/mobile/Base/NativeBinding");
            jmethodID myMethodId = env->GetStaticMethodID(cls, "getCameraParams", "()Ljava/util/List;");
            if(myMethodId != 0) {
                jvalue myArgs[0];
                jobject myList = env->CallStaticObjectMethod (cls, myMethodId, myArgs);
                jclass listClass = env->GetObjectClass(myList);
                jmethodID getMethod = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");

                jobject myInt = (jobject)env->CallObjectMethod(myList, getMethod, 0);
                jclass myIntegerClass = env->GetObjectClass(myInt);
                jmethodID intValueMethod = env->GetMethodID(myIntegerClass, "intValue", "()I");
                myCameraInfo.textureID = (jint)env->CallIntMethod(myInt, intValueMethod, 0);

                myInt = (jobject)env->CallObjectMethod(myList, getMethod, 1);
                myCameraInfo.width = (jint)env->CallIntMethod(myInt, intValueMethod, 0);

                myInt = (jobject)env->CallObjectMethod(myList, getMethod, 2);
                myCameraInfo.height = (jint)env->CallIntMethod(myInt, intValueMethod, 0);

                myInt = (jobject)env->CallObjectMethod(myList, getMethod, 3);
                myCameraInfo.texturewidth = (jint)env->CallIntMethod(myInt, intValueMethod, 0);

                myInt = (jobject)env->CallObjectMethod(myList, getMethod, 4);
                myCameraInfo.textureheight = (jint)env->CallIntMethod(myInt, intValueMethod, 0);

            } else {
                AC_WARNING  << "Sorry, java-getCameraParams not found";
            }
        }
        return myCameraInfo;
    }
    void AndroidMobileSDK::updateCameraTexture() {
        if (env) {
            jclass cls = env->FindClass("com/artcom/mobile/Base/NativeBinding");
            jmethodID myMethodId = env->GetStaticMethodID(cls, "updateCameraTexture", "()V");
            if(myMethodId != 0) {
                jvalue myArgs[0];
                env->CallStaticVoidMethodA (cls, myMethodId, myArgs);
            } else {
                AC_WARNING  << "Sorry, java-updateCameraTextures not found";
            }
        }
    }

    void AndroidMobileSDK::startCameraCapture(bool theColorConversionFlag) {
        if (env) {
            jclass cls = env->FindClass("com/artcom/mobile/Base/NativeBinding");
            jmethodID myMethodId = env->GetStaticMethodID(cls, "startCamera", "(Z)V");
            if(myMethodId != 0) {
                jvalue myArgs[1];
                myArgs[0].b = theColorConversionFlag;
                env->CallStaticVoidMethodA (cls, myMethodId, myArgs);
                AC_PRINT << "start camera capture";
            } else {
                AC_WARNING  << "Sorry, java-startCamera not found";
            }
        }
    }

    void AndroidMobileSDK::stopCameraCapture() {
        if (env) {
            jclass cls = env->FindClass("com/artcom/mobile/Base/NativeBinding");
            jmethodID myMethodId = env->GetStaticMethodID(cls, "stopCamera", "()V");
            if(myMethodId != 0) {
                jvalue myArgs[0];
                env->CallStaticVoidMethodA (cls, myMethodId, myArgs);
                AC_PRINT << "stop camera capture";
            } else {
                AC_WARNING  << "Sorry, java-stopCamera not found";
            }
        }
    }
    bool AndroidMobileSDK::isCameraCapturing() {
        bool myResult = false;
        if (env) {
            jclass cls = env->FindClass("com/artcom/mobile/Base/NativeBinding");
            jmethodID myMethodId = env->GetStaticMethodID(cls, "isCameraCapturing", "()Z");
            if(myMethodId != 0) {
                jvalue myArgs[0];
                myResult = env->CallStaticBooleanMethod (cls, myMethodId, myArgs);
            } else {
                AC_WARNING  << "Sorry, java-isCameraCapturing not found";
            }
        }
        return myResult;
    }
}
