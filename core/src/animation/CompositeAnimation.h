// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_animation_CompositeAnimation_h_included_
#define _ac_mobile_animation_CompositeAnimation_h_included_

#include <vector>

#include "Animation.h"

namespace animation {
    class CompositeAnimation : public Animation {
    public:
        CompositeAnimation();
        virtual ~CompositeAnimation();
        void add(AnimationPtr theAnimation);
        virtual void childDurationChanged() = 0;
        virtual void finish(const masl::UInt64 theTime);
        virtual void cancel();

    protected:
        std::vector<AnimationPtr> _myChildren;
    };
};

#endif

