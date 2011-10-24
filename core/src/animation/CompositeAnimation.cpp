// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "CompositeAnimation.h"

namespace animation {

    CompositeAnimation::CompositeAnimation() : Animation() {
    }

    CompositeAnimation::~CompositeAnimation() {
    }

    void CompositeAnimation::add(AnimationPtr theAnimation) {
        _myChildren.push_back(theAnimation);
        AnimationPtr myThis = shared_from_this();
        childDurationChanged();
    };

    void CompositeAnimation::cancel() {
        for (std::vector<AnimationPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            if ((*it)->isRunning()) {
                (*it)->cancel();
            }
        }
        Animation::cancel();
    }


    void CompositeAnimation::finish(const masl::UInt64 theTime) {
        for (std::vector<AnimationPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            (*it)->finish(theTime);
        }
        Animation::finish(theTime);
    }
}

