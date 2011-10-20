#ifndef _ac_mobile_masl_AudioEngine_h_included_
#define _ac_mobile_masl_AudioEngine_h_included_

#include <vector>

#include "Singleton.h"
#include "Logger.h"
#include "Ptr.h"

namespace masl {
    class AudioEngine {
    public:
        virtual unsigned int playEffect(const std::string & theFile) const = 0;
    };
    typedef masl::Ptr<AudioEngine> AudioEnginePtr;

    class AudioEngineSingleton : public masl::Singleton<AudioEngineSingleton> {
    public:
        // we want a real singleton -> we need a non purevirtual out-of-line method other than dtor
        virtual void setAudioEngine(AudioEnginePtr theAudioEngine);
        const AudioEnginePtr & ae() const { AC_PRINT << "get audio engine " << _myAudioEngine; return _myAudioEngine; };
    private:
        AudioEnginePtr _myAudioEngine;
    };
};
#endif
