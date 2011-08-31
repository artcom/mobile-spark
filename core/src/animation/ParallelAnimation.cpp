#include "ParallelAnimation.h"

namespace animation {

    ParallelAnimation::ParallelAnimation():CompositeAnimation() {
    }

    ParallelAnimation::~ParallelAnimation() {
    }

    // duration = max(map(children, duration))
    void ParallelAnimation::childDurationChanged() {
        masl::UInt64 d = 0;
        for (std::vector<AnimationPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            masl::UInt64 c = (*it)->getDuration();
            if (c > d) {
                d = c;
            }
        }
        _myDuration = d;
    }

    void ParallelAnimation::play(const masl::UInt64 theStartTime, const bool theComeToAnEndFlag) {
        CompositeAnimation::play(theStartTime, theComeToAnEndFlag);
        if (!theComeToAnEndFlag) {
            for (std::vector<AnimationPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
                if (isRunning()) {
                    (*it)->play(theStartTime);
                }
            }
        }
    }

    void ParallelAnimation::doFrame(const masl::UInt64 theTime) {
        bool notFinished = false;
        for (std::vector<AnimationPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            if (isRunning() && (*it)->isRunning()) {
                (*it)->doFrame(theTime);
                notFinished |= (*it)->isRunning();
            }
        }

        if (!notFinished && isRunning()) {
            finishAnimation(theTime);
        }
    }
}

