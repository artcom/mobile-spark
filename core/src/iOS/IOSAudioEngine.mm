#include "IOSAudioEngine.h"
#include <masl/AssetProvider.h>
#include "CDAudioManager.h"

#include <masl/CallStack.h>

namespace ios
{
    CDBufferManager *bufferManager=nil;
    CDSoundEngine* soundEngine=nil;
    CDAudioManager *am=nil;

    IOSAudioEngine::IOSAudioEngine()
    : AudioEngine()
    {
        am = [CDAudioManager sharedManager];
		soundEngine = am.soundEngine;
		bufferManager = [[CDBufferManager alloc] initWithEngine:soundEngine];
    }

    IOSAudioEngine::~IOSAudioEngine()
    {
        am = nil;
        soundEngine = nil;
        bufferManager = nil;
    }

	void 
    IOSAudioEngine::playBackgroundMusic(const std::string & theFile, bool isLoop) const {
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        NSString *iosFileName = [NSString stringWithCString:myFoundFile.c_str() encoding:NSUTF8StringEncoding];
        AC_DEBUG << "playBackgroundMusic " << myFoundFile;
        [am playBackgroundMusic:iosFileName loop:isLoop];
	}

	void 
    IOSAudioEngine::stopBackgroundMusic() const {
        AC_DEBUG << "stopBackgroundMusic";
        [am stopBackgroundMusic];
	}

	void 
    IOSAudioEngine::pauseBackgroundMusic() const {
        AC_DEBUG << "pauseBackgroundMusic";
        [am pauseBackgroundMusic];
	}

	void 
    IOSAudioEngine::resumeBackgroundMusic() const {
        AC_DEBUG << "resumeBackgroundMusic";
        [am resumeBackgroundMusic];
	}

	void 
    IOSAudioEngine::rewindBackgroundMusic() const {
        AC_DEBUG << "rewindBackgroundMusic";
        [am rewindBackgroundMusic];
	}

	bool 
    IOSAudioEngine::isBackgroundMusicPlaying() const {
        AC_DEBUG << "isBackgroundMusicPlaying";
        return [am isBackgroundMusicPlaying];
    }

	float 
    IOSAudioEngine::getBackgroundMusicVolume() const {
        AC_DEBUG << "getBackgroundMusicVolume";
        return am.backgroundMusic.volume;
    }

	void 
    IOSAudioEngine::setBackgroundMusicVolume(float volume) const {
        AC_DEBUG << "setBackgroundMusicVolume " << volume;
        am.backgroundMusic.volume = volume;
	}


    ////////////////////////////////////////Effect Sounds
    //returns soundid
    unsigned int
    IOSAudioEngine::playEffect(const std::string & theFile) const {
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        AC_DEBUG << "play Effect " << myFoundFile;
		NSString *iosFileName = [NSString stringWithCString:myFoundFile.c_str() encoding:NSUTF8StringEncoding];

        int soundId = [bufferManager bufferForFile:iosFileName create:YES];
        if (soundId != kCDNoBuffer) {
            return [soundEngine playSound:soundId sourceGroupId:0 pitch:1 pan:0 gain:1 loop:false];
        } else {
            return CD_MUTE;
        }	
    }

	void 
    IOSAudioEngine::stopEffect(unsigned int nSoundId) const {
        AC_DEBUG << "stop Effect with soundId" << nSoundId;
        [soundEngine stopSound:nSoundId];
	}

	float 
    IOSAudioEngine::getEffectsVolume() const {
        AC_DEBUG << "getEffectsVolume";
        return am.soundEngine.masterGain;
	}

	void 
    IOSAudioEngine::setEffectsVolume(float volume) const {
        AC_DEBUG << "setEffectsVolume " << volume;
        am.soundEngine.masterGain = volume;
	}

	void 
    IOSAudioEngine::preloadEffect(const std::string & theFile) const {
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        NSString *iosFileName = [NSString stringWithCString:myFoundFile.c_str() encoding:NSUTF8StringEncoding];
        AC_DEBUG << "preload Effect " << myFoundFile;
        int soundId = [bufferManager bufferForFile:iosFileName create:YES];
        if (soundId == kCDNoBuffer) AC_ERROR << "Denshion::SimpleAudioEngine sound failed to preload "<< iosFileName;
	}

	void 
    IOSAudioEngine::unloadEffect(const std::string & theFile) const {
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        NSString *iosFileName = [NSString stringWithCString:myFoundFile.c_str() encoding:NSUTF8StringEncoding];
        AC_DEBUG << "unload Effect " << iosFileName;
        [bufferManager releaseBufferForFile:iosFileName];	
    }
    

	void 
    IOSAudioEngine::end() const {
        AC_DEBUG << "end";
        am = nil;
        [CDAudioManager end];
        [bufferManager release];
	}

}
