#ifndef _ac_mobile_masl_AudioEngine_h_included_
#define _ac_mobile_masl_AudioEngine_h_included_

#include <vector>
#include <jni.h>

#include "Singleton.h"
#include "Logger.h"
#include "Ptr.h"

#ifdef ANDROID
extern "C" {
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved);
}
#endif

namespace masl {
    class AudioEngine : public masl::Singleton<AudioEngine> {
    public:
        unsigned int playEffect(const std::string & theFile) const;
    };
    typedef masl::Ptr<AudioEngine> AudioEnginePtr;
};
#endif
