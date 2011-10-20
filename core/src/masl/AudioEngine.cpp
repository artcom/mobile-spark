#include "AudioEngine.h"


namespace masl {
    void
    AudioEngineSingleton::setAudioEngine(AudioEnginePtr theAudioEngine) {
        _myAudioEngine = theAudioEngine;
    };
}

