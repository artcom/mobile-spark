// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "AndroidMovieEngine.h"
#include "AndroidMobileSDK.h"

#include <masl/AssetProvider.h>

#include <jni.h>

namespace android {

    AndroidMovieEngine::AndroidMovieEngine(const std::string & theJavaActivity)
        : MovieEngine(), javaActivity_(theJavaActivity)
    {
    }

    AndroidMovieEngine::~AndroidMovieEngine() {
    }

    void 
    AndroidMovieEngine::playMovie(void* theMovieWidget, const std::string & theSrc) {
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theSrc);
        jclass cls = env->FindClass(javaActivity_.c_str());
        jmethodID myMethodId = env->GetStaticMethodID(cls, "playMovie", "(I;Ljava/lang/String)V");
        if(myMethodId != 0) {
            jstring StringArg = env->NewStringUTF(myFoundFile.c_str());
            env->CallStaticVoidMethod(cls, myMethodId, int(theMovieWidget), StringArg);
        } else {
            AC_WARNING  << "Sorry, playMovie not found";
        }
    }

    void 
    AndroidMovieEngine::stopMovie(void* theMovieWidget) {
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        jmethodID myMethodId = env->GetStaticMethodID(cls, "stopMovie", "(I)V");
        if(myMethodId != 0) {
            env->CallStaticVoidMethod(cls, myMethodId, int(theMovieWidget));
        } else {
            AC_WARNING  << "Sorry, stopMovie not found";
        }
    }

    void 
    AndroidMovieEngine::pauseMovie(void* theMovieWidget) {
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        jmethodID myMethodId = env->GetStaticMethodID(cls, "pauseMovie", "(I)V");
        if(myMethodId != 0) {
            env->CallStaticVoidMethod(cls, myMethodId, int(theMovieWidget));
        } else {
            AC_WARNING  << "Sorry, pauseMovie not found";
        }
    }

    void 
    AndroidMovieEngine::updateMovieTexture(void* theMovieWidget) {
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        jmethodID myMethodId = env->GetStaticMethodID(cls, "updateMovieTexture", "(I)V");
        if(myMethodId != 0) {
            env->CallStaticVoidMethod(cls, myMethodId, int(theMovieWidget));
        } else {
            AC_WARNING  << "Sorry, updateMovieTexture not found";
        }
    }

    const masl::VideoInfo 
    AndroidMovieEngine::getMovieInfo(void* theMovieWidget) const {
        masl::VideoInfo myMovieInfo;
        myMovieInfo.textureID=0;
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        jmethodID myMethodId = env->GetStaticMethodID(cls, "getMovieInfo", "(I)Ljava/util/List;");
        if(myMethodId != 0) {
            jvalue myArgs[0];
            jobject myList = env->CallStaticObjectMethod (cls, myMethodId, myArgs);
            jclass listClass = env->GetObjectClass(myList);
            jmethodID getMethod = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");

            jobject myInt = (jobject)env->CallObjectMethod(myList, getMethod, 0);
            jclass myIntegerClass = env->GetObjectClass(myInt);
            jmethodID intValueMethod = env->GetMethodID(myIntegerClass, "intValue", "()I");
            myMovieInfo.textureID = env->CallIntMethod(myInt, intValueMethod, 0);

            myInt = (jobject)env->CallObjectMethod(myList, getMethod, 1);
            myMovieInfo.width = (jint)env->CallIntMethod(myInt, intValueMethod, 0);

            myInt = (jobject)env->CallObjectMethod(myList, getMethod, 2);
            myMovieInfo.height = (jint)env->CallIntMethod(myInt, intValueMethod, 0);
        } else {
            AC_WARNING  << "Sorry, java-getMovieParams not found";
        }
        return myMovieInfo;
    }

    bool 
    AndroidMovieEngine::isMoviePlaying(void* theMovieWidget) const {
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        bool isPlaying = false;
        jmethodID myMethodId = env->GetStaticMethodID(cls, "isMoviePlaying", "(I)Z");
        if(myMethodId != 0) {
            isPlaying = env->CallStaticBooleanMethod(cls, myMethodId, int(theMovieWidget));
        } else {
            AC_WARNING  << "Sorry, isMoviePlaying not found";
        }
        return isPlaying;
    }

    void 
    AndroidMovieEngine::setMovieVolume(void* theMovieWidget, float theVolume) {
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        jmethodID myMethodId = env->GetStaticMethodID(cls, "setMovieVolume", "(I;F)V");
        if(myMethodId != 0) {
            env->CallStaticVoidMethod(cls, myMethodId, int(theMovieWidget), theVolume);
        } else {
            AC_WARNING  << "Sorry, setMovieVolume not found";
        }
    }
}


