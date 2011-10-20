#ifndef _included_mobile_ios_IOSAudioEngine_
#define _included_mobile_ios_IOSAudioEngine_

#include <masl/AudioEngine.h>

namespace ios {
class IOSAudioEngine : public masl::AudioEngine {
public:
    IOSAudioEngine();
    virtual ~IOSAudioEngine();

    unsigned int playEffect(const std::string & theFile) const;
private:
};
typedef masl::Ptr<IOSAudioEngine> IOSAudioEngine;
};

#endif
