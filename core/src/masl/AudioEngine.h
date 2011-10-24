// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_masl_AudioEngine_h_included_
#define _ac_mobile_masl_AudioEngine_h_included_

#include <vector>

#include "Singleton.h"
#include "Logger.h"
#include "Ptr.h"

namespace masl {
    class AudioEngine {
    public:
        virtual void playBackgroundMusic(const std::string & theFile, bool isLoop) const = 0;
        virtual void stopBackgroundMusic() const = 0;
        virtual void pauseBackgroundMusic() const = 0;
        virtual void resumeBackgroundMusic() const = 0;
        virtual void rewindBackgroundMusic() const = 0;
        virtual bool isBackgroundMusicPlaying() const = 0;
        virtual float getBackgroundMusicVolume() const = 0;
        virtual void setBackgroundMusicVolume(float volume) const = 0;

        virtual unsigned int playEffect(const std::string & theFile) const = 0;
        virtual void stopEffect(unsigned int nSoundId) const = 0;
        virtual float getEffectsVolume() const = 0;
        virtual void setEffectsVolume(float volume) const = 0;
        virtual void preloadEffect(const std::string & theFile) const = 0;
        virtual void unloadEffect(const std::string & theFile) const = 0;

        virtual void end() const = 0;
    };
    typedef masl::Ptr<AudioEngine> AudioEnginePtr;

    class AudioEngineSingleton : public masl::Singleton<AudioEngineSingleton> {
    public:
        // we want a real singleton -> we need a non purevirtual out-of-line method other than dtor
        virtual void setAudioEngine(AudioEnginePtr theAudioEngine);
        const AudioEnginePtr & ae() const { return _myAudioEngine; };
    private:
        AudioEnginePtr _myAudioEngine;
    };
};
#endif
