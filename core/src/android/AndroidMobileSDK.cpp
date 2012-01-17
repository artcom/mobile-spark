// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "AndroidMobileSDK.h"
#include <masl/Logger.h>

using namespace masl;

namespace android {
    AndroidMobileSDK::AndroidMobileSDK() {

    }

    AndroidMobileSDK::~AndroidMobileSDK() {

    }
    bool AndroidMobileSDK::playMovie(const std::string & theURL) {}
    void AndroidMobileSDK::stopMovie() {}
    void AndroidMobileSDK::pauseMovie() {}
    void AndroidMobileSDK::resetMovie() {}
    
    void AndroidMobileSDK::vibrate(long theDurationMillisec) {
        if (env) {
            jclass cls = env->FindClass("com/artcom/mobile/Base/NativeBinding");
            jmethodID myMethodId = env->GetStaticMethodID(cls, "vibrate", "(I)V");
            if(myMethodId != 0) {
                jvalue myArgs[2];
                myArgs[0].i =theDurationMillisec ;
                env->CallStaticVoidMethodA (cls, myMethodId, myArgs);
            } else {
                AC_WARNING  << "Sorry, java-vibrate not found";
            }
        }       
    }
    bool AndroidMobileSDK::loadTextureFromFile(const std::string & filename, unsigned int & textureId, 
                                               unsigned int & width, unsigned int & height, 
                                               unsigned int & real_width, unsigned int & real_height, 
                                               bool & hasAlpha) {
        if (env) {
            std::string myFullPath = filename;
            bool myFileIsonSDCardFlag = masl::searchFile(filename, myFullPath);
            env->PushLocalFrame(10); // i can only guess about the capacity for the local reference frame [http://java.sun.com/docs/books/jni/html/refs.html] (vs)
            jclass cls = env->FindClass("com/artcom/mobile/Base/NativeBinding");
            jmethodID myMethodId = env->GetStaticMethodID(cls, "loadTextureFromFile", "(Ljava/lang/String;Z)Ljava/util/List;");
            if(myMethodId != 0) {
                env->PushLocalFrame(10); // i can only guess about the capacity for the local reference frame [http://java.sun.com/docs/books/jni/html/refs.html] (vs)                
                jvalue myArgs[2];
                myArgs[0].l =  env->NewStringUTF(filename.c_str());
                myArgs[1].b =  myFileIsonSDCardFlag;
                jobject myList = env->CallStaticObjectMethodA (cls, myMethodId, myArgs);
                jclass listClass = env->GetObjectClass(myList);
                jmethodID getMethod = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");

                jobject myInt = (jobject)env->CallObjectMethod(myList, getMethod, 0);
                jclass myIntegerClass = env->GetObjectClass(myInt);

                jmethodID intValueMethod = env->GetMethodID(myIntegerClass, "intValue", "()I");
                textureId = env->CallIntMethod(myInt, intValueMethod, 0);

                myInt = (jobject)env->CallObjectMethod(myList, getMethod, 1);
                width = (jint)env->CallIntMethod(myInt, intValueMethod, 0);

                myInt = (jobject)env->CallObjectMethod(myList, getMethod, 2);
                height = (jint)env->CallIntMethod(myInt, intValueMethod, 0);
                
                myInt = (jobject)env->CallObjectMethod(myList, getMethod, 3);
                real_width = (jint)env->CallIntMethod(myInt, intValueMethod, 0);

                myInt = (jobject)env->CallObjectMethod(myList, getMethod, 4);
                real_height = (jint)env->CallIntMethod(myInt, intValueMethod, 0);

                myInt = (jobject)env->CallObjectMethod(myList, getMethod, 5);
                hasAlpha = (jint)env->CallIntMethod(myInt, intValueMethod, 0) == 1;    
                
            } else {
                AC_WARNING  << "Sorry, java-loadTextureFromXXX not found";                
            }
            env->PopLocalFrame(NULL);            
        }
        return textureId != -1;   
    }
    
    masl::TextInfo AndroidMobileSDK::renderText(const std::string & theMessage, unsigned int theTextureId, int theFontSize,
                                                vector4 theColor, int theMaxWidth, int theMaxHeight, const std::string & theAlign,
                                                const std::string & theFontPath, int theLineHeight, int theStartIndex,
                                                bool & mirrorFlag) {
        mirrorFlag = true;
        masl::TextInfo myTextInfo;
        if (env) {
            env->PushLocalFrame(10); // i can only guess about the capacity for the local reference frame [http://java.sun.com/docs/books/jni/html/refs.html] (vs)
            jclass cls = env->FindClass("com/artcom/mobile/Base/NativeBinding");
            jmethodID myMethodId = env->GetStaticMethodID(cls, "renderText", "(Ljava/lang/String;II[IIILjava/lang/String;Ljava/lang/String;II)Ljava/util/List;");
            if(myMethodId != 0) {
                jvalue myArgs[10];
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
                myArgs[8].i =  theLineHeight;
                myArgs[9].i =  theStartIndex;
                jobject myList = env->CallStaticObjectMethodA (cls, myMethodId, myArgs);
                jclass listClass = env->GetObjectClass(myList);
                jmethodID getMethod = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");

                jobject myInt = (jobject)env->CallObjectMethod(myList, getMethod, 0);
                jclass myIntegerClass = env->GetObjectClass(myInt);

                jmethodID intValueMethod = env->GetMethodID(myIntegerClass, "intValue", "()I");
                myTextInfo.textureID = env->CallIntMethod(myInt, intValueMethod, 0);

                myInt = (jobject)env->CallObjectMethod(myList, getMethod, 1);
                myTextInfo.width = (jint)env->CallIntMethod(myInt, intValueMethod, 0);

                myInt = (jobject)env->CallObjectMethod(myList, getMethod, 2);
                myTextInfo.height = (jint)env->CallIntMethod(myInt, intValueMethod, 0);
                
                myInt = (jobject)env->CallObjectMethod(myList, getMethod, 3);
                myTextInfo.renderedGlyphIndex = (jint)env->CallIntMethod(myInt, intValueMethod, 0);    
                            
            } else {
                AC_WARNING  << "Sorry, java-rendertext not found";
            }
            env->PopLocalFrame(NULL);
        }
        return myTextInfo;
    }

    void AndroidMobileSDK::freezeMobileOrientation(const std::string & theOrientation) {
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
                myCameraInfo.textureID = env->CallIntMethod(myInt, intValueMethod, 0);

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
                AC_INFO << "start camera capture";
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
                AC_INFO << "stop camera capture";
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
    
    void AndroidMobileSDK::exit() {
        if (env) {
            jclass cls = env->FindClass("com/artcom/mobile/Base/NativeBinding");
            jmethodID myMethodId = env->GetStaticMethodID(cls, "exit", "()V");
            if(myMethodId != 0) {
                jvalue myArgs[0];
                env->CallStaticVoidMethodA (cls, myMethodId, myArgs);
                AC_INFO << "exit program";
            } else {
                AC_WARNING  << "Sorry, java-exit not found";
            }
        }
    }
    
    std::string AndroidMobileSDK::getOrientation(){
    	return "";	
    }

}
