#include "AnimationManager.h"

#include <masl/Logger.h>

namespace animation {

    void AnimationManager::play(AnimationPtr theAnimation) {
        _myAnimations.push_back(theAnimation);
        theAnimation->play(_myAnimationTime);
    }
    
    void AnimationManager::doFrame(const long theCurrentMillis) {
        _myAnimationTime = theCurrentMillis;
        for (std::vector<AnimationPtr>::iterator it = _myAnimations.begin(); it != _myAnimations.end(); ++it) {
            AnimationPtr myAnimation = (*it);
            if (myAnimation->isRunning()) {
                myAnimation->doFrame(_myAnimationTime);
            }
        }
        removeFinished();
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

