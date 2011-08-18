#ifndef _ac_mobile_animation_AnimationManager_h_included_
#define _ac_mobile_animation_AnimationManager_h_included_

#include <vector>
#include <masl/Singleton.h>
#include "Animation.h"

namespace animation {

    class AnimationManager : public masl::Singleton<AnimationManager> {
    public: 
        unsigned int animationCount() const { return _myAnimations.size(); };
        void play(AnimationPtr theAnimation);
        bool isPlaying() const { return _myAnimations.size() > 0; };
        void doFrame(const long theCurrentMillis);
        void init(const long theCurrentMillis) { _myAnimationTime = theCurrentMillis; };

    private:
        std::vector<AnimationPtr> _myAnimations;
        long _myAnimationTime;

        void removeFinished();
    };
};
#endif

