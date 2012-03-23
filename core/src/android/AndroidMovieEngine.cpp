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
        jmethodID myMethodId = env->GetStaticMethodID(cls, "play", "(Ljava/lang/String;Z)V");
        if(myMethodId != 0) {
			jstring StringArg = env->NewStringUTF(myFoundFile.c_str());
			env->CallStaticVoidMethod(cls, myMethodId, StringArg);
        } else {
            AC_WARNING  << "Sorry, playMovie not found";
        }
	}

	void 
    AndroidMovieEngine::stopMovie(void* theMovieWidget) {
	}

	void 
    AndroidMovieEngine::pauseMovie(void* theMovieWidget) {
	}

	void 
    AndroidMovieEngine::resetMovie(void* theMovieWidget) {
	}

	void 
    AndroidMovieEngine::updateMovieTexture(void* theMovieWidget) {
	}

    const masl::VideoInfo 
    AndroidMovieEngine::getMovieInfo(void* theMovieWidget) const {
        masl::VideoInfo myMovieInfo;
        return myMovieInfo;
	}

	bool 
    AndroidMovieEngine::isMoviePlaying(void* theMovieWidget) const {
        return false;
	}

	void 
    AndroidMovieEngine::setMovieVolume(void* theMovieWidget, float theVolume) {
	}
}


