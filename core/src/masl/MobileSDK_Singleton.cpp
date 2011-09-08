/* __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2008, ART+COM AG Berlin, Germany <www.artcom.de>
//
// These coded instructions, statements, and computer programs contain
// proprietary information of ART+COM AG Berlin, and are copy protected
// by law. They may be used, modified and redistributed under the terms
// of GNU General Public License referenced below.
//
// Alternative licensing without the obligations of the GPL is
// available upon request.
//
// GPL v3 Licensing:
//
// This file is part of the ART+COM Y60 Platform.
//
// ART+COM Y60 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// ART+COM Y60 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with ART+COM Y60.  If not, see <http://www.gnu.org/licenses/>.
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
*/

#include "MobileSDK_Singleton.h"
#include "Logger.h"


namespace masl {
    MobileSDK_Singleton::MobileSDK_Singleton() {}
    MobileSDK_Singleton::~MobileSDK_Singleton() {}   

    int MobileSDK_Singleton::renderText(const std::string & theMessage, int theTextureId, int theFontSize, vector4 theColor) {
        int myTextureId = -1;
#ifdef __ANDROID__        
        if (env) {
            jclass cls = env->FindClass("com/artcom/mobile/Base/NativeBinding");            
            jmethodID myMethodId = env->GetStaticMethodID(cls, "renderText", "(Ljava/lang/String;II[I)I");
            if(myMethodId != 0) {
               jvalue myArgs[4];
                myArgs[0].l =  env->NewStringUTF(theMessage.c_str());
                myArgs[1].i = theTextureId;
                myArgs[2].i = theFontSize;
                jintArray jI = env->NewIntArray(4);
                jint array[] = { theColor[0] * 255, theColor[1] * 255, theColor[2] * 255, theColor[3] * 255};
                env->SetIntArrayRegion(jI, 0 , 4, array);
                myArgs[3].l = jI;

                myTextureId = env->CallStaticIntMethodA (cls, myMethodId, myArgs);                
            } else {
                AC_WARNING  << "Sorry, java-rendertext not found";                
            }
        }
#endif        
        return myTextureId;
    }     
    
    CameraInfo MobileSDK_Singleton::getCameraSpec() {
        CameraInfo myCameraInfo;
        myCameraInfo.textureID=0;
#ifdef __ANDROID__        
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
#endif                
        return myCameraInfo;
    }
    void MobileSDK_Singleton::updateCameraTexture() {
	
#ifdef __ANDROID__        
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
#endif        
    }
    
    void MobileSDK_Singleton::startCameraCapture() {
#ifdef __ANDROID__        
        if (env) {
            jclass cls = env->FindClass("com/artcom/mobile/Base/NativeBinding");            
            jmethodID myMethodId = env->GetStaticMethodID(cls, "startCamera", "()V");
            if(myMethodId != 0) {
                jvalue myArgs[0];
                env->CallStaticVoidMethodA (cls, myMethodId, myArgs);
                AC_PRINT << "start camera capture";
            } else {
                AC_WARNING  << "Sorry, java-startCamera not found";                
            }
        }
#endif        

    }
        
    void MobileSDK_Singleton::stopCameraCapture() {
#ifdef __ANDROID__        
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
#endif        
                
    }
    bool MobileSDK_Singleton::isCameraCapturing() {
        bool myResult = false;
#ifdef __ANDROID__        
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
#endif        
        return myResult;
    }


};
