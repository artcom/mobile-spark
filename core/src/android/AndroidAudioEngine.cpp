// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

//Copyright (c) 2010-2011 cocos2d-x.org
//Copyright (c) 2009      On-Core
// 
//http://www.cocos2d-x.org
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in
//all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.

#include "AndroidAudioEngine.h"
#include "AndroidMobileSDK.h"

#include <masl/AssetProvider.h>

#include <jni.h>

namespace android {
    //supported audio formats: http://developer.android.com/guide/appendix/media-formats.html#core

    AndroidAudioEngine::AndroidAudioEngine(const std::string & theJavaActivity)
        : AudioEngine(), javaActivity_(theJavaActivity)
    {
    }

    AndroidAudioEngine::~AndroidAudioEngine() {
    }


    //////////////////////////////////////////BackgroundMusic
	void 
    AndroidAudioEngine::playBackgroundMusic(const std::string & theFile, bool isLoop) const {
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        AC_DEBUG << "playBackgroundMusic " << myFoundFile;
        jclass cls = env->FindClass(javaActivity_.c_str());
        jmethodID myMethodId = env->GetStaticMethodID(cls, "playBackgroundMusic", "(Ljava/lang/String;Z)V");
		if (myMethodId) {
			jstring StringArg = env->NewStringUTF(myFoundFile.c_str());
			env->CallStaticVoidMethod(cls, myMethodId, StringArg, isLoop);
		}
	}

	void 
    AndroidAudioEngine::stopBackgroundMusic() const {
        AC_DEBUG << "stopBackgroundMusic";
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        jmethodID myMethodId = env->GetStaticMethodID(cls, "stopBackgroundMusic", "()V");
		if (myMethodId) {
			env->CallStaticVoidMethod(cls, myMethodId);
		}
	}

	void 
    AndroidAudioEngine::pauseBackgroundMusic() const {
        AC_DEBUG << "pauseBackgroundMusic";
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        jmethodID myMethodId = env->GetStaticMethodID(cls, "pauseBackgroundMusic", "()V");
		if (myMethodId) {
			env->CallStaticVoidMethod(cls, myMethodId);
		}
	}

	void 
    AndroidAudioEngine::resumeBackgroundMusic() const {
        AC_DEBUG << "resumeBackgroundMusic";
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        jmethodID myMethodId = env->GetStaticMethodID(cls, "resumeBackgroundMusic", "()V");
		if (myMethodId) {
			env->CallStaticVoidMethod(cls, myMethodId);
		}
	}

	void 
    AndroidAudioEngine::rewindBackgroundMusic() const {
        AC_DEBUG << "rewindBackgroundMusic";
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        jmethodID myMethodId = env->GetStaticMethodID(cls, "rewindBackgroundMusic", "()V");
		if (myMethodId) {
			env->CallStaticVoidMethod(cls, myMethodId);
		}
	}

	bool 
    AndroidAudioEngine::isBackgroundMusicPlaying() const {
        AC_DEBUG << "isBackgroundMusicPlaying";
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        jmethodID myMethodId = env->GetStaticMethodID(cls, "isBackgroundMusicPlaying", "()Z");
		jboolean ret = false;
		if (myMethodId) {
			ret = env->CallStaticBooleanMethod(cls, myMethodId);
		}
		return ret;
	}

	float 
    AndroidAudioEngine::getBackgroundMusicVolume() const {
        AC_DEBUG << "getBackgroundMusicVolume";
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        jmethodID myMethodId = env->GetStaticMethodID(cls, "getBackgroundMusicVolume", "()F");
		jfloat ret = 0.0;
		if (myMethodId) {
			ret = env->CallStaticFloatMethod(cls, myMethodId);
		}
		return ret;
	}

	void 
    AndroidAudioEngine::setBackgroundMusicVolume(float theVolume) const {
        AC_DEBUG << "setBackgroundMusicVolume " << theVolume;
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        jmethodID myMethodId = env->GetStaticMethodID(cls, "setBackgroundMusicVolume", "(F)V");
		if (myMethodId) {
			env->CallStaticVoidMethod(cls, myMethodId, theVolume);
		}
	}


    ////////////////////////////////////////Effect Sounds
    //returns soundid
    unsigned int
    AndroidAudioEngine::playEffect(const std::string & theFile) const {
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        AC_DEBUG << "play Effect " << myFoundFile;
		int ret = 0;
        jmethodID myMethodId = env->GetStaticMethodID(cls, "playEffect", "(Ljava/lang/String;)I");
		if (myMethodId) {
			jstring StringArg = env->NewStringUTF(myFoundFile.c_str());
			ret = env->CallStaticIntMethod(cls, myMethodId, StringArg);
		}
		return ret;
    }

	void 
    AndroidAudioEngine::stopEffect(unsigned int theSoundId) const {
        AC_DEBUG << "stop Effect with soundId" << theSoundId;
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        jmethodID myMethodId = env->GetStaticMethodID(cls, "stopEffect", "(I)V");
		if (myMethodId) {
			env->CallStaticVoidMethod(cls, myMethodId, static_cast<int>(theSoundId));
		}
	}

	float 
    AndroidAudioEngine::getEffectsVolume() const {
        AC_DEBUG << "getEffectsVolume";
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        jmethodID myMethodId = env->GetStaticMethodID(cls, "getEffectsVolume", "()F");
		jfloat ret = -1.0;
		if (myMethodId) {
			ret = env->CallStaticFloatMethod(cls, myMethodId);
		}
		return ret;
	}

	void 
    AndroidAudioEngine::setEffectsVolume(float theVolume) const {
        AC_DEBUG << "setEffectsVolume " << theVolume;
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        jmethodID myMethodId = env->GetStaticMethodID(cls, "setEffectsVolume", "(F)V");
		if (myMethodId) {
			env->CallStaticVoidMethod(cls, myMethodId, theVolume);
		}
	}

	void 
    AndroidAudioEngine::preloadEffect(const std::string & theFile) const {
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        AC_DEBUG << "preload Effect " << myFoundFile;
        jmethodID myMethodId = env->GetStaticMethodID(cls, "preloadEffect", "(Ljava/lang/String;)V");
		if (myMethodId) {
			jstring StringArg = env->NewStringUTF(myFoundFile.c_str());
			env->CallStaticVoidMethod(cls, myMethodId, StringArg);
		}
	}

	void 
    AndroidAudioEngine::unloadEffect(const std::string & theFile) const {
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        AC_DEBUG << "unload Effect " << myFoundFile;
        jmethodID myMethodId = env->GetStaticMethodID(cls, "unloadEffect", "(Ljava/lang/String;)V");
		if (myMethodId) {
			jstring StringArg = env->NewStringUTF(myFoundFile.c_str());
			env->CallStaticVoidMethod(cls, myMethodId, StringArg);
		}
	}
    

	void 
    AndroidAudioEngine::end() const {
        JNIEnv *env = boost::static_pointer_cast<android::AndroidMobileSDK>(masl::MobileSDK_Singleton::get().getNative())->env;
        jclass cls = env->FindClass(javaActivity_.c_str());
        AC_DEBUG << "end";
        jmethodID myMethodId = env->GetStaticMethodID(cls, "end", "()V");
		if (myMethodId) {
			env->CallStaticVoidMethod(cls, myMethodId);
		}
	}
}

