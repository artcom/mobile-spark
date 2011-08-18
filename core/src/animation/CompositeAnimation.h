#ifndef _ac_mobile_animation_CompositeAnimation_h_included_
#define _ac_mobile_animation_CompositeAnimation_h_included_

#include <vector>

#include "Animation.h"

namespace animation {
    class CompositeAnimation : public Animation {
    public:
        void add(AnimationPtr theAnimation);
        virtual void childDurationChanged() = 0;
        virtual void finish(const long theTime);
        virtual void cancel();

    protected:
        std::vector<AnimationPtr> _myChildren;
    };
};

#endif

