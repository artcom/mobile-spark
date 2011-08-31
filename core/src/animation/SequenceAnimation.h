#ifndef _ac_mobile_animation_SequenceAnimation_h_included_
#define _ac_mobile_animation_SequenceAnimation_h_included_

#include "CompositeAnimation.h"

namespace animation {
    class SequenceAnimation : public CompositeAnimation {
    public:
        SequenceAnimation();
        virtual ~SequenceAnimation();
        virtual void doFrame(const masl::UInt64 theTime);
        virtual void childDurationChanged();
        virtual void play(const masl::UInt64 theStartTime, const bool theComeToAnEndFlag = false);
    private:
        unsigned int _myCurrent;
    };

    typedef boost::shared_ptr<SequenceAnimation> SequenceAnimationPtr;
};

#endif

