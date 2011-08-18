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

    void Animation::doFrame(const long theCurrentMillis) {
        _myProgress = (float)(theCurrentMillis - _myStartTime)/(float)(_myDuration);
        if (_myProgress >= 1) {
            //AC_PRINT << _myId << "..................... stop it";
            finish(theCurrentMillis);
        }
    }

    void Animation::play(const long theStartTime, const bool theComeToAnEndFlag) {
        //AC_PRINT << _myId << "..........play it";
        _myStartTime = theStartTime;
        _myProgress = 0.0;
        _myRunning = true;
        _myFinished = false;

        //TODO: callOnPlay
    }

    void Animation::cancel() {
        //AC_PRINT << _myId << "..........cancel it";
        _myRunning = false;
        _myFinished = false;
        //TODO: callOnCancel();
    }

    void Animation::finish(const long theTime) {
        //AC_PRINT << _myId << "..........finish it";
        if (_myFinished) {
            return;
        }
        if (!_myRunning) {
            //when composit animations are finished before child animations started
            //child animations should be started (with comeToAnEndFlag = true) and finished immediately
            play(theTime, true); 
        }
        finishAnimation(theTime);
    }

    void Animation::finishAnimation(const long theTime) {
        //AC_PRINT << _myId << "..........finish animation";
        _myProgress = 1.0; 
        //TODO: callOnFinish();
        if (_myLoop) {
            //AC_PRINT << _myId << "..........loop: restart animation";
            play(theTime);
        } else {
            _myRunning = false;
            _myFinished = true;
        }
    }
}

