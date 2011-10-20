#ifndef _included_mobile_ios_IOSAudioEngine_
#define _included_mobile_ios_IOSAudioEngine_

#include <masl/AudioEngine.h>

namespace ios {
    class IOSAudioEngine : public masl::AudioEngine {
    public:
        IOSAudioEngine();
        virtual ~IOSAudioEngine();

        void playBackgroundMusic(const char *path, bool isLoop) const;
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
        void preloadEffect(const char *path) const;
        void unloadEffect(const char* path) const;

        void end() const;

    private:
    };
    typedef masl::Ptr<IOSAudioEngine> IOSAudioEngine;
};

#endif
