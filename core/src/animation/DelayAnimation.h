#ifndef _ac_mobile_animation_DelayAnimation_h_included_
#define _ac_mobile_animation_DelayAnimation_h_included_

#include "Animation.h"

namespace animation {
    class DelayAnimation : public Animation {
    public:
        DelayAnimation(const masl::UInt64 theDuration=1000);
        virtual ~DelayAnimation();
    };

    typedef boost::shared_ptr<DelayAnimation> DelayAnimationPtr;
};

#endif
