#ifndef _ac_mobile_animation_ParallelAnimation_h_included_
#define _ac_mobile_animation_ParallelAnimation_h_included_

#include "CompositeAnimation.h"

namespace animation {
    class ParallelAnimation : public CompositeAnimation {
    public:
        ParallelAnimation();
        virtual ~ParallelAnimation();
        virtual void doFrame(const masl::UInt64 theTime);
        virtual void childDurationChanged();
        virtual void play(const masl::UInt64 theStartTime, const bool theComeToAnEndFlag = false);
    };

    typedef boost::shared_ptr<ParallelAnimation> ParallelAnimationPtr;
};

#endif

