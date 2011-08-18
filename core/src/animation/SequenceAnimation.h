#ifndef _ac_mobile_animation_SequenceAnimation_h_included_
#define _ac_mobile_animation_SequenceAnimation_h_included_

#include "CompositeAnimation.h"

namespace animation {
    class SequenceAnimation : public CompositeAnimation {
    public:
        SequenceAnimation();
        virtual ~SequenceAnimation();
        virtual void doFrame(const long theTime);
        virtual void childDurationChanged();
        virtual void play(const long theStartTime, const bool theComeToAnEndFlag = false);
    private:
        unsigned int _myCurrent;
    };

    typedef boost::shared_ptr<SequenceAnimation> SequenceAnimationPtr;
};

#endif

