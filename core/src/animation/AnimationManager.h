// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_animation_AnimationManager_h_included_
#define _ac_mobile_animation_AnimationManager_h_included_

#include <vector>
#include <masl/Singleton.h>
#include <masl/Settings.h>
#include "Animation.h"

namespace animation {

    class AnimationManager : public masl::Singleton<AnimationManager> {
    public:
        virtual unsigned int animationCount() const;
        void play(AnimationPtr theAnimation);
        bool isPlaying() const { return _myAnimations.size() > 0; };
        void doFrame(const masl::UInt64 theCurrentMillis);
        void init(const masl::UInt64 theCurrentMillis) { _myAnimationTime = theCurrentMillis; };
        masl::UInt64 getTime() const { return _myAnimationTime; };

    private:
        std::vector<AnimationPtr> _myAnimations;
        std::vector<AnimationPtr> _myOnFrameCollectedAnimations;
        bool _myIsInOnFrame;
        masl::UInt64 _myAnimationTime;

        void removeFinished();
    };
};
#endif

