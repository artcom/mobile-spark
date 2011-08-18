#include "Animation.h"

#include <masl/Logger.h>

namespace animation {

    unsigned int Animation::idCounter = 0;

    Animation::Animation(const long theDuration) : 
        _myDuration(theDuration),
        _myId(idCounter++),
        _myRunning(false),
        _myFinished(false),
        _myLoop(false) {
    }

    Animation::~Animation() {}

    void Animation::play(const long theStartTime) {
        //AC_PRINT << _myId << "..........play it";
        _myStartTime = theStartTime;
        _myRunning = true;
        _myFinished = false;
    }

    void Animation::doFrame(const long theCurrentMillis) {
        _myProgress = (float)(theCurrentMillis - _myStartTime)/(float)(_myDuration);
        if (_myProgress >= 1) {
            //AC_PRINT << _myId << "..................... stop it";
            finish();
            if (_myLoop) {
                //AC_PRINT << _myId << " ........ loop it";
                play(theCurrentMillis);
            } else  {
                _myRunning = false;
                _myFinished = true;
            }
        }
    }

    void Animation::finish() {
        _myProgress = 1.0; 
    }
}

