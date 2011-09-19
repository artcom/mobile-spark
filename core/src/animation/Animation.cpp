#include "Animation.h"

#include <masl/Logger.h>

namespace animation {

    unsigned int Animation::idCounter = 0;

    Animation::Animation(const masl::UInt64 theDuration, EasingFunctionPtr theEasing) :
        _myDuration(theDuration),
        _myEasingFunction(theEasing),
        _myId(idCounter++),
        _myRunning(false),
        _myFinished(false),
        _myLoop(false) {
    }

    Animation::~Animation() {
        //AC_PRINT << "destruct animation " << _myId;
    }

    void Animation::doFrame(const masl::UInt64 theCurrentMillis) {
        _myProgressTime = theCurrentMillis - _myStartTime;
        _myProgress = _myEasingFunction(_myProgressTime/(float)_myDuration);
        AC_TRACE << _myId << " progress is now " << _myProgress;
        if (_myProgressTime >= _myDuration) {
            AC_DEBUG << _myId << "..................... stop it";
            finish(theCurrentMillis);
        }
    }

    void Animation::play(const masl::UInt64 theStartTime, const bool theComeToAnEndFlag) {
        AC_DEBUG << _myId << "..........play it";
        _myStartTime = theStartTime;
        _myProgressTime = 0.0;
        _myProgress = _myEasingFunction(_myProgressTime);
        _myRunning = true;
        _myFinished = false;

        if (_myOnPlay) { _myOnPlay->execute(); }
    }

    void Animation::cancel() {
        //AC_PRINT << _myId << "..........cancel it";
        _myRunning = false;
        _myFinished = false;
        if (_myOnCancel) { _myOnCancel->execute(); }
    }

    void Animation::finish(const masl::UInt64 theTime) {
        //AC_PRINT << _myId << "..........finish it";
        if (_myFinished) {
            return;
        }
        if (!_myRunning) {
            //when composite animations are finished before child animations started
            //child animations should be started (with comeToAnEndFlag = true) and finished immediately
            play(theTime, true);
        }
        finishAnimation(theTime);
    }

    void Animation::finishAnimation(const masl::UInt64 theTime) {
        //AC_PRINT << _myId << "..........finish animation";
        _myProgressTime = _myDuration;
        _myProgress = _myEasingFunction(1.0);
        if (_myOnFinish) { _myOnFinish->execute(); }
        if (_myLoop) {
            //AC_PRINT << _myId << "..........loop: restart animation";
            play(theTime);
        } else {
            _myRunning = false;
            _myFinished = true;
        }
    }
}

