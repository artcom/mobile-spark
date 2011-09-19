#include "CompositeAnimation.h"

namespace animation {

    CompositeAnimation::CompositeAnimation() : Animation() {
    }

    CompositeAnimation::~CompositeAnimation() {
    }

    void CompositeAnimation::add(AnimationPtr theAnimation) {
        _myChildren.push_back(theAnimation);
        AnimationPtr myThis = shared_from_this();
        theAnimation->setParent(AnimationWeakPtr(myThis));
        childDurationChanged();
    };

    void CompositeAnimation::cancel() {
        for (std::vector<AnimationPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            if ((*it)->isRunning()) {
                (*it)->cancel();
            }
        }
        Animation::cancel();
    }


    void CompositeAnimation::finish(const masl::UInt64 theTime) {
        for (std::vector<AnimationPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            (*it)->finish(theTime);
        }
        Animation::finish(theTime);
    }
}

