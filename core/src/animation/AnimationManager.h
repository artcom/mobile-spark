#ifndef _ac_mobile_animation_AnimationManager_h_included_
#define _ac_mobile_animation_AnimationManager_h_included_

#include <vector>
#include <masl/Singleton.h>
#include <masl/Settings.h>
#include "Animation.h"

namespace animation {

    class AnimationManager : public masl::Singleton<AnimationManager> {
    public: 
        virtual unsigned int animationCount() const { return _myAnimations.size(); };
        void play(AnimationPtr theAnimation);
        bool isPlaying() const { return _myAnimations.size() > 0; };
        void doFrame(const masl::UInt64 theCurrentMillis);
        void init(const masl::UInt64 theCurrentMillis) { _myAnimationTime = theCurrentMillis; };

    private:
        std::vector<AnimationPtr> _myAnimations;
        masl::UInt64 _myAnimationTime;

        void removeFinished();
    };
};
#endif

