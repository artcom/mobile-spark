#include "AnimationManager.h"

#include <masl/Logger.h>

namespace animation {

    void AnimationManager::play(AnimationPtr theAnimation) {
        _myAnimations.push_back(theAnimation);
        theAnimation->play(_myAnimationTime);
    }
    
    void AnimationManager::doFrame(const long theCurrentMillis) {
        _myAnimationTime = theCurrentMillis;
        //AC_PRINT << "AnimationManager::doFrame";
        for (std::vector<AnimationPtr>::iterator it = _myAnimations.begin(); it != _myAnimations.end(); ++it) {
            AnimationPtr myAnimation = (*it);
            if (myAnimation->isRunning()) {
                myAnimation->doFrame(_myAnimationTime);
            }
        }
        //AC_PRINT << "AnimationManager::doFrame 1";
        removeFinished();
        //AC_PRINT << "AnimationManager::doFrame 2";
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
            //AC_PRINT << "erase animation " << (**it)->getId();
            _myAnimations.erase(*it);
        }
    }
}

