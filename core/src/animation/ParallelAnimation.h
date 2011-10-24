// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_animation_ParallelAnimation_h_included_
#define _ac_mobile_animation_ParallelAnimation_h_included_

#include "CompositeAnimation.h"

namespace animation {
    class ParallelAnimation : public CompositeAnimation {
    public:
        ParallelAnimation();
        virtual ~ParallelAnimation();
        virtual void doFrame(const masl::UInt64 theTime);
        virtual void childDurationChanged();
        virtual void play(const masl::UInt64 theStartTime, const bool theComeToAnEndFlag = false);
    };

    typedef masl::Ptr<ParallelAnimation> ParallelAnimationPtr;
};

#endif

