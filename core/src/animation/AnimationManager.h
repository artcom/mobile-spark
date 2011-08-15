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
        void doFrame(const unsigned long long theTime);

    private:
        std::vector<AnimationPtr> _myAnimations;
        unsigned long long _myAnimationTime;

        void removeFinished();
    };
};
#endif

