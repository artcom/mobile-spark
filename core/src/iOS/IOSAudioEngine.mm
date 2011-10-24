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
