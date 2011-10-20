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

        void playBackgroundMusic(const std::string & theFile, bool isLoop) const;
        void stopBackgroundMusic() const;
        void pauseBackgroundMusic() const;
        void resumeBackgroundMusic() const;
        void rewindBackgroundMusic() const;
        bool isBackgroundMusicPlaying() const;
        float getBackgroundMusicVolume() const;
        void setBackgroundMusicVolume(float volume) const;

        unsigned int playEffect(const std::string & theFile) const;
        void stopEffect(unsigned int nSoundId) const;
        float getEffectsVolume() const;
        void setEffectsVolume(float volume) const;
        void preloadEffect(const std::string & theFile) const;
        void unloadEffect(const std::string & theFile) const;

        void end() const;
    private:
        std::string javaActivity_;
    };

    typedef masl::Ptr<AndroidAudioEngine> AndroidAudioEnginePtr;
};

#endif
