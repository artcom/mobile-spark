#include "SequenceAnimation.h"

namespace animation {

    // duration = sum(map(children, duration))
    void SequenceAnimation::childDurationChanged() {
        long d = 0;
        for (std::vector<AnimationPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            d += (*it)->getDuration();
        }
        _myDuration = d;
    }

    // start to play this animation
    // plays the first animation
    void SequenceAnimation::play(const long theStartTime, const bool theComeToAnEndFlag) {
        CompositeAnimation::play(theStartTime, theComeToAnEndFlag);
        if (!theComeToAnEndFlag && isRunning()) {
            _myCurrent = 0;
            if (_myChildren.size() > 0) {
                _myChildren[0]->play(theStartTime);
            }
        }
    }

    // iterate through child animations
    void SequenceAnimation::doFrame(const long theTime) {
        if (_myCurrent >= _myChildren.size()) {
            return;
        }
        _myChildren[_myCurrent]->doFrame(theTime);
        if (!_myChildren[_myCurrent]->isRunning() && isRunning()) {
            _myCurrent++;
            if (_myCurrent < _myChildren.size()) {
                _myChildren[_myCurrent]->play(theTime);
            } else {
                finishAnimation(theTime);
            }
        }
    }
}

