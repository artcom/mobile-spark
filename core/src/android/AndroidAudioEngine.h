#ifndef _included_mobile_android_AndroidAudioEngine_
#define _included_mobile_android_AndroidAudioEngine_

#include <jni.h>
#include <masl/AudioEngine.h>

#ifdef ANDROID
extern "C" {
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved);
}
#endif



namespace android {
    class AndroidAudioEngine : public masl::AudioEngine {
    public:
        AndroidAudioEngine(const std::string & theJavaActivity);
        virtual ~AndroidAudioEngine();

        unsigned int playEffect(const std::string & theFile) const;
    private:
        std::string javaActivity_;
    };

    typedef masl::Ptr<AndroidAudioEngine> AndroidAudioEnginePtr;
};

#endif
