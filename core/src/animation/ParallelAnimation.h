#ifndef _ac_mobile_animation_ParallelAnimation_h_included_
#define _ac_mobile_animation_ParallelAnimation_h_included_

#include "CompositeAnimation.h"

namespace animation {
    class ParallelAnimation : public CompositeAnimation {
    public:
        virtual void doFrame(const long theTime);
        virtual void childDurationChanged();
        virtual void play(const long theStartTime, const bool theComeToAnEndFlag = false);
    };
};

#endif

