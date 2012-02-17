// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "Animation.h"

#include <masl/Logger.h>

namespace animation {

    DEFINE_EXCEPTION(AnimationException, masl::Exception);

    unsigned int Animation::idCounter = 0;

    Animation::Animation(const masl::UInt64 theDuration, EasingFunctionPtr theEasing) :
        _myDuration(theDuration),
        _myEasingFunction(theEasing),
        _myId(idCounter++),
        _myRunning(false),
        _myFinished(false),
        _myLoop(false),
        _myVerboseFlag(false) {
    }

    Animation::~Animation() {
    }

    void Animation::doFrame(const masl::UInt64 theCurrentMillis) {
        _myProgressTime = theCurrentMillis - _myStartTime;
        _myProgress = _myEasingFunction(_myProgressTime/(float)_myDuration);
        if (_myProgressTime >= _myDuration) {
            AC_DEBUG << _myId << "..................... stop animation";
            finish(theCurrentMillis);
        }
    }

    void Animation::play(const masl::UInt64 theStartTime, const bool theComeToAnEndFlag) {
        _myStartTime = theStartTime;
        _myProgressTime = 0.0;
        _myProgress = _myEasingFunction(_myProgressTime);
        _myRunning = true;
        _myFinished = false;

        if (_myOnPlay) { _myOnPlay->execute(); }
    }

    void Animation::cancel() {
        _myRunning = false;
        _myFinished = true;
        if (_myOnCancel) { _myOnCancel->execute(); }
    }

    void Animation::finish(const masl::UInt64 theTime) {
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
        AC_DEBUG << _myId << "..........finish animation";
        _myProgressTime = _myDuration;
        _myProgress = _myEasingFunction(1.0);
        if (_myOnFinish) { _myOnFinish->execute(); }
        if (_myLoop) {
            AC_DEBUG << _myId << "..........loop: restart animation";
            play(theTime);
        } else {
            _myRunning = false;
            _myFinished = true;
        }
    }
}

