#include "IOSAudioEngine.h"
#include <masl/AssetProvider.h>

namespace ios
{
    IOSAudioEngine::IOSAudioEngine()
    : AudioEngine()
    {
    }

    IOSAudioEngine::~IOSAudioEngine()
    {
    }

	void 
    IOSAudioEngine::playBackgroundMusic(const std::string & theFile, bool isLoop) const {
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        AC_DEBUG << "playBackgroundMusic " << myFoundFile;
        //TODO
	}

	void 
    IOSAudioEngine::stopBackgroundMusic() const {
        AC_DEBUG << "stopBackgroundMusic";
        //TODO
	}

	void 
    IOSAudioEngine::pauseBackgroundMusic() const {
        AC_DEBUG << "pauseBackgroundMusic";
        //TODO
	}

	void 
    IOSAudioEngine::resumeBackgroundMusic() const {
        AC_DEBUG << "resumeBackgroundMusic";
        //TODO
	}

	void 
    IOSAudioEngine::rewindBackgroundMusic() const {
        AC_DEBUG << "rewindBackgroundMusic";
        //TODO
	}

	bool 
    IOSAudioEngine::isBackgroundMusicPlaying() const {
        AC_DEBUG << "isBackgroundMusicPlaying";
		bool ret = false;
        //TODO
		return ret;
	}

	float 
    IOSAudioEngine::getBackgroundMusicVolume() const {
        AC_DEBUG << "getBackgroundMusicVolume";
		float ret = 0.0;
        //TODO
		return ret;
	}

	void 
    IOSAudioEngine::setBackgroundMusicVolume(float volume) const {
        AC_DEBUG << "setBackgroundMusicVolume " << volume;
        //TODO
	}


    ////////////////////////////////////////Effect Sounds
    //returns soundid
    unsigned int
    IOSAudioEngine::playEffect(const std::string & theFile) const {
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        AC_DEBUG << "play Effect " << myFoundFile;
		int ret = 0;
        //TODO
		return (unsigned int)ret;
    }

	void 
    IOSAudioEngine::stopEffect(unsigned int nSoundId) const {
        AC_DEBUG << "stop Effect with soundId" << nSoundId;
        //TODO
	}

	float 
    IOSAudioEngine::getEffectsVolume() const {
        AC_DEBUG << "getEffectsVolume";
		float ret = -1.0;
        //TODO
		return ret;
	}

	void 
    IOSAudioEngine::setEffectsVolume(float volume) const {
        AC_DEBUG << "setEffectsVolume " << volume;
        //TODO
	}

	void 
    IOSAudioEngine::preloadEffect(const std::string & theFile) const {
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        AC_DEBUG << "preload Effect " << myFoundFile;
        //TODO
	}

	void 
    IOSAudioEngine::unloadEffect(const std::string & theFile) const {
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        AC_DEBUG << "unload Effect " << myFoundFile;
        //TODO
	}
    

	void 
    IOSAudioEngine::end() const {
        AC_DEBUG << "end";
        //TODO
	}

}
