#include "CompositeAnimation.h"

namespace animation {

    void CompositeAnimation::add(AnimationPtr theAnimation) {
        _myChildren.push_back(theAnimation);
        theAnimation->setParent(AnimationPtr(this));
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


    void CompositeAnimation::finish(const long theTime) {
        for (std::vector<AnimationPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            (*it)->finish(theTime);
        }
        Animation::finish(theTime);
    }
}

