#include "Animation.h"

#include <masl/Logger.h>

namespace animation {

    Animation::Animation(const long theDuration) : 
        _myRunning(false),
        _myDuration(theDuration) {
    }

    Animation::~Animation() {}

    void Animation::play(const long theStartTime) {
        _myStartTime = theStartTime;
        _myRunning = true;
    }

    void Animation::doFrame(const long theCurrentMillis) {
        _myProgress = (float)(theCurrentMillis - _myStartTime)/(float)(_myDuration);
        if (_myProgress > 1) {
            //AC_PRINT << "..................... stop it";
            _myRunning = false;
        }
    }
}

