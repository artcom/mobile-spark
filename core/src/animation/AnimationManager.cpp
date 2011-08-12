#include "AnimationManager.h"

namespace animation {

    void AnimationManager::play(AnimationPtr theAnimation) {
        _myAnimations.push_back(theAnimation);
        theAnimation->play();
    }
    
    void AnimationManager::doFrame(const unsigned long long theTime) {
        _myAnimationTime = theTime * 1000;
        for (std::vector<AnimationPtr>::iterator it = _myAnimations.begin(); it != _myAnimations.end(); ++it) {
            AnimationPtr myAnimation = (*it);
            if (myAnimation->isRunning()) {
                myAnimation->doFrame(_myAnimationTime);
            }
        }
        removeFinished();

    }

    void AnimationManager::removeFinished() {
        for (std::vector<AnimationPtr>::iterator it = _myAnimations.begin(); it != _myAnimations.end(); ++it) {
            AnimationPtr myAnimation = (*it);
            if (!myAnimation->isRunning()) {
                _myAnimations.erase(it);
            }
        }
    }
}

