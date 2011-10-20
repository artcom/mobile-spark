#include "AndroidAudioEngine.h"

#include <masl/AssetProvider.h>


static JavaVM *gJavaVM = 0;
static jclass classOfSparkViewerActivity = 0;
JNIEnv *env = 0;

#ifdef ANDROID
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    gJavaVM = vm;
    return JNI_VERSION_1_6;
}
#endif


namespace android {
    //supported audio formats: http://developer.android.com/guide/appendix/media-formats.html#core

	static jmethodID getMethodID(const std::string & theJavaActivity, const char *methodName, const char *paramCode) {
		jmethodID ret = 0;
		if (gJavaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
			AC_ERROR << "Failed to get the environment using GetEnv()";
			return 0;
		}
		if (gJavaVM->AttachCurrentThread(&env, 0) < 0) {
			AC_ERROR << "Failed to get the environment using AttachCurrentThread()";
			return 0;
		}
		classOfSparkViewerActivity = env->FindClass(theJavaActivity.c_str());
		if (! classOfSparkViewerActivity) {
			AC_ERROR << "Failed to find class of " << theJavaActivity.c_str();
			return 0;
		}
		if (env != 0 && classOfSparkViewerActivity != 0) {
			ret = env->GetStaticMethodID(classOfSparkViewerActivity, methodName, paramCode);
		}
		if (! ret) {
			AC_ERROR << "get method id of " << methodName << " error";
		}
		return ret;
	}


    AndroidAudioEngine::AndroidAudioEngine(const std::string & theJavaActivity)
        : AudioEngine(), javaActivity_(theJavaActivity)
    {
    }

    AndroidAudioEngine::~AndroidAudioEngine() {
    }


    //////////////////////////////////////////BackgroundMusic
	void 
    AndroidAudioEngine::playBackgroundMusic(const std::string & theFile, bool isLoop) const {
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        AC_DEBUG << "playBackgroundMusic " << myFoundFile;
		jmethodID playBackgroundMusicMethodID = getMethodID(javaActivity_, "playBackgroundMusic", "(Ljava/lang/String;Z)V");
		if (playBackgroundMusicMethodID) {
			jstring StringArg = env->NewStringUTF(myFoundFile.c_str());
			env->CallStaticVoidMethod(classOfSparkViewerActivity, playBackgroundMusicMethodID, StringArg, isLoop);
			//env->ReleaseStringUTFChars(StringArg, path);
		}
	}

	void 
    AndroidAudioEngine::stopBackgroundMusic() const {
        AC_DEBUG << "stopBackgroundMusic";
		jmethodID stopBackgroundMusicMethodID = getMethodID(javaActivity_, "stopBackgroundMusic", "()V");
		if (stopBackgroundMusicMethodID) {
			env->CallStaticVoidMethod(classOfSparkViewerActivity, stopBackgroundMusicMethodID);
		}
	}

	void 
    AndroidAudioEngine::pauseBackgroundMusic() const {
        AC_DEBUG << "pauseBackgroundMusic";
		jmethodID pauseBackgroundMusicMethodID = getMethodID(javaActivity_, "pauseBackgroundMusic", "()V");
		if (pauseBackgroundMusicMethodID) {
			env->CallStaticVoidMethod(classOfSparkViewerActivity, pauseBackgroundMusicMethodID);
		}
	}

	void 
    AndroidAudioEngine::resumeBackgroundMusic() const {
        AC_DEBUG << "resumeBackgroundMusic";
		jmethodID resumeBackgroundMusicMethodID = getMethodID(javaActivity_, "resumeBackgroundMusic", "()V");
		if (resumeBackgroundMusicMethodID) {
			env->CallStaticVoidMethod(classOfSparkViewerActivity, resumeBackgroundMusicMethodID);
		}
	}

	void 
    AndroidAudioEngine::rewindBackgroundMusic() const {
        AC_DEBUG << "rewindBackgroundMusic";
		jmethodID rewindBackgroundMusicMethodID = getMethodID(javaActivity_, "rewindBackgroundMusic", "()V");
		if (rewindBackgroundMusicMethodID) {
			env->CallStaticVoidMethod(classOfSparkViewerActivity, rewindBackgroundMusicMethodID);
		}
	}

	bool 
    AndroidAudioEngine::isBackgroundMusicPlaying() const {
        AC_DEBUG << "isBackgroundMusicPlaying";
		jmethodID isBackgroundMusicPlayingMethodID = getMethodID(javaActivity_, "isBackgroundMusicPlaying", "()Z");
		jboolean ret = false;
		if (isBackgroundMusicPlayingMethodID) {
			ret = env->CallStaticBooleanMethod(classOfSparkViewerActivity, isBackgroundMusicPlayingMethodID);
		}
		return ret;
	}

	float 
    AndroidAudioEngine::getBackgroundMusicVolume() const {
        AC_DEBUG << "getBackgroundMusicVolume";
		jmethodID getBackgroundMusicVolumeID = getMethodID(javaActivity_, "getBackgroundMusicVolume", "()F");
		jfloat ret = 0.0;
		if (getBackgroundMusicVolumeID) {
			ret = env->CallStaticFloatMethod(classOfSparkViewerActivity, getBackgroundMusicVolumeID);
		}
		return ret;
	}

	void 
    AndroidAudioEngine::setBackgroundMusicVolume(float volume) const {
        AC_DEBUG << "setBackgroundMusicVolume " << volume;
		jmethodID setBackgroundMusicVolumeMethodID = getMethodID(javaActivity_, "setBackgroundMusicVolume", "(F)V");
		if (setBackgroundMusicVolumeMethodID) {
			env->CallStaticVoidMethod(classOfSparkViewerActivity, setBackgroundMusicVolumeMethodID, volume);
		}
	}


    ////////////////////////////////////////Effect Sounds
    //returns soundid
    unsigned int
    AndroidAudioEngine::playEffect(const std::string & theFile) const {
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        AC_DEBUG << "play Effect " << myFoundFile;
		int ret = 0;
		jmethodID playEffectMethodID = getMethodID(javaActivity_, "playEffect", "(Ljava/lang/String;)I");
		if (playEffectMethodID) {
			jstring StringArg = env->NewStringUTF(myFoundFile.c_str());
			ret = env->CallStaticIntMethod(classOfSparkViewerActivity, playEffectMethodID, StringArg);
		}
		return (unsigned int)ret;
    }

	void 
    AndroidAudioEngine::stopEffect(unsigned int nSoundId) const {
        AC_DEBUG << "stop Effect with soundId" << nSoundId;
		jmethodID stopEffectMethodID = getMethodID(javaActivity_, "stopEffect", "(I)V");
		if (stopEffectMethodID) {
			env->CallStaticVoidMethod(classOfSparkViewerActivity, stopEffectMethodID, (int)nSoundId);
		}
	}

	float 
    AndroidAudioEngine::getEffectsVolume() const {
        AC_DEBUG << "getEffectsVolume";
		jmethodID getEffectsVolumeMethodID = getMethodID(javaActivity_, "getEffectsVolume", "()F");
		jfloat ret = -1.0;
		if (getEffectsVolumeMethodID) {
			ret = env->CallStaticFloatMethod(classOfSparkViewerActivity, getEffectsVolumeMethodID);
		}
		return ret;
	}

	void 
    AndroidAudioEngine::setEffectsVolume(float volume) const {
        AC_DEBUG << "setEffectsVolume " << volume;
		jmethodID setEffectsVolumeMethodID = getMethodID(javaActivity_, "setEffectsVolume", "(F)V");
		if (setEffectsVolumeMethodID) {
			env->CallStaticVoidMethod(classOfSparkViewerActivity, setEffectsVolumeMethodID, volume);
		}
	}

	void 
    AndroidAudioEngine::preloadEffect(const std::string & theFile) const {
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        AC_DEBUG << "preload Effect " << myFoundFile;
		jmethodID preloadEffectMethodID = getMethodID(javaActivity_, "preloadEffect", "(Ljava/lang/String;)V");
		if (preloadEffectMethodID) {
			jstring StringArg = env->NewStringUTF(myFoundFile.c_str());
			env->CallStaticVoidMethod(classOfSparkViewerActivity, preloadEffectMethodID, StringArg);
		}
	}

	void 
    AndroidAudioEngine::unloadEffect(const std::string & theFile) const {
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        AC_DEBUG << "unload Effect " << myFoundFile;
		jmethodID unloadEffectMethodID = getMethodID(javaActivity_, "unloadEffect", "(Ljava/lang/String;)V");
		if (unloadEffectMethodID) {
			jstring StringArg = env->NewStringUTF(myFoundFile.c_str());
			env->CallStaticVoidMethod(classOfSparkViewerActivity, unloadEffectMethodID, StringArg);
		}
	}
    

	void 
    AndroidAudioEngine::end() const {
        AC_DEBUG << "end";
		jmethodID endMethodID = getMethodID(javaActivity_, "end", "()V");
		if (endMethodID) {
			env->CallStaticVoidMethod(classOfSparkViewerActivity, endMethodID);
		}
	}
}

