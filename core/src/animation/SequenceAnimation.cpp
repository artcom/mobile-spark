// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "SequenceAnimation.h"

namespace animation {

    SequenceAnimation::SequenceAnimation() : CompositeAnimation() {
    }

    SequenceAnimation::~SequenceAnimation() {
    }

    // duration = sum(map(children, duration))
    void SequenceAnimation::childDurationChanged() {
        masl::UInt64 d = 0;
        for (std::vector<AnimationPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            d += (*it)->getDuration();
        }
        _myDuration = d;
    }

    // start to play this animation
    // plays the first animation
    void SequenceAnimation::play(const masl::UInt64 theStartTime, const bool theComeToAnEndFlag) {
        CompositeAnimation::play(theStartTime, theComeToAnEndFlag);
        if (!theComeToAnEndFlag && isRunning()) {
            _myCurrent = 0;
            if (_myChildren.size() > 0) {
                _myChildren[0]->play(theStartTime);
            }
        }
    }

    // iterate through child animations
    void SequenceAnimation::doFrame(const masl::UInt64 theTime) {
        if (_myCurrent >= _myChildren.size()) {
            finishAnimation(theTime);
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

