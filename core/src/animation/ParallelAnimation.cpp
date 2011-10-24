// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "ParallelAnimation.h"

namespace animation {

    ParallelAnimation::ParallelAnimation():CompositeAnimation() {
    }

    ParallelAnimation::~ParallelAnimation() {
    }

    // duration = max(map(children, duration))
    void ParallelAnimation::childDurationChanged() {
        masl::UInt64 d = 0;
        for (std::vector<AnimationPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            masl::UInt64 c = (*it)->getDuration();
            if (c > d) {
                d = c;
            }
        }
        _myDuration = d;
    }

    void ParallelAnimation::play(const masl::UInt64 theStartTime, const bool theComeToAnEndFlag) {
        CompositeAnimation::play(theStartTime, theComeToAnEndFlag);
        if (!theComeToAnEndFlag) {
            for (std::vector<AnimationPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
                if (isRunning()) {
                    (*it)->play(theStartTime);
                }
            }
        }
    }

    void ParallelAnimation::doFrame(const masl::UInt64 theTime) {
        bool notFinished = false;
        for (std::vector<AnimationPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            if (isRunning() && (*it)->isRunning()) {
                (*it)->doFrame(theTime);
                notFinished |= (*it)->isRunning();
            }
        }

        if (!notFinished && isRunning()) {
            finishAnimation(theTime);
        }
    }
}

