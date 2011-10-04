#include "AnimationManager.h"

#include <masl/Logger.h>

namespace animation {

    unsigned int AnimationManager::animationCount() const {
        return _myAnimations.size();
    }

    void AnimationManager::play(AnimationPtr theAnimation) {
        if (_myIsInOnFrame) {
            _myOnFrameCollectedAnimations.push_back(theAnimation);
        } else {
            _myAnimations.push_back(theAnimation);
            theAnimation->play(_myAnimationTime);
        }
    }

    void AnimationManager::doFrame(const masl::UInt64 theCurrentMillis) {
        _myIsInOnFrame = true;
        _myAnimationTime = theCurrentMillis;
        for (std::vector<AnimationPtr>::iterator it = _myAnimations.begin(); it != _myAnimations.end(); ++it) {
            AnimationPtr myAnimation = (*it);
            if (myAnimation->isRunning()) {
                myAnimation->doFrame(_myAnimationTime);
            }
        }
        removeFinished();
        _myIsInOnFrame = false;
        for (std::vector<AnimationPtr>::iterator it = _myOnFrameCollectedAnimations.begin(); it != _myOnFrameCollectedAnimations.end(); ++it) {
            _myAnimations.push_back(*it);
            (*it)->play(_myAnimationTime);
        }
        _myOnFrameCollectedAnimations.clear();
    }

    void AnimationManager::removeFinished() {
        std::vector<std::vector<AnimationPtr>::iterator> myToErase;
        for (std::vector<AnimationPtr>::iterator it = _myAnimations.begin(); it != _myAnimations.end(); ++it) {
            AnimationPtr myAnimation = (*it);
            if (!myAnimation->isRunning()) {
                myToErase.push_back(it);
            }
        }
        for (std::vector<std::vector<AnimationPtr>::iterator>::iterator it = myToErase.begin(); it != myToErase.end(); ++it) {
            _myAnimations.erase(*it);
        }
    }
}

