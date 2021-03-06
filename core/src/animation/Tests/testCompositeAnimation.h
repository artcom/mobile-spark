// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_asl_test_CompositeAnimation_h_included_
#define _ac_mobile_asl_test_CompositeAnimation_h_included_

#include <string>
#include <algorithm>
#include <masl/UnitTest.h>

#include "../AnimationManager.h"
#include "../PropertyAnimation.h"
#include "../ParallelAnimation.h"
#include "../SequenceAnimation.h"


namespace animation {
    class CompositeAnimation_UnitTest : public UnitTest {
    public:
        CompositeAnimation_UnitTest() : UnitTest("CompositeAnimation_UnitTest") {  }
        void run() {
            perform_EmptySequenceTest();
            perform_EmptyParallelTest();
            perform_SequenceTest();
            perform_ParallelTest();
            perform_SequenceFinishBeforeFirstFrameTest();
            perform_SequenceFinishAfterFirstAnimationTest();
            perform_SequenceCancelTest();
            perform_ParallelFinishTest();
            perform_parallelCancelTest();
        }

        class Object {
            private:
                float x;
                float y;
            public:
                Object() { x = 0; y = 0; };
                void setX(float theX) { x = theX;};
                void setY(float theY) { y = theY;};
                float getX() { return x;};
                float getY() { return y;};
        };
        typedef masl::Ptr<Object> ObjectPtr;
        typedef masl::WeakPtr<Object> ObjectWeakPtr;

        //animations
        typedef void (Object::* ObjectPropertySetterFunction)(float);
        typedef PropertyAnimation<ObjectWeakPtr, ObjectPropertySetterFunction> ObjectPropertyAnimation;
        typedef masl::Ptr<ObjectPropertyAnimation>  ObjectPropertyAnimationPtr;

        void perform_EmptySequenceTest() {
            AnimationManager::get().init(0);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);
            SequenceAnimationPtr mySequence = SequenceAnimationPtr(new SequenceAnimation);
            ENSURE_MSG(mySequence, "mySequence should not be null");

            AnimationManager::get().play(mySequence);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 1);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), true);

            AnimationManager::get().doFrame(0);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);
        }

        void perform_EmptyParallelTest() {
            AnimationManager::get().init(0);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);
            ParallelAnimationPtr myParallel = ParallelAnimationPtr(new ParallelAnimation);
            ENSURE_MSG(myParallel, "myParallel should not be null");

            AnimationManager::get().play(myParallel);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 1);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), true);

            AnimationManager::get().doFrame(0);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);
        }

        void perform_SequenceTest() {
            AnimationManager::get().init(0);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);

            SequenceAnimationPtr mySequence = SequenceAnimationPtr(new SequenceAnimation);
            ENSURE_MSG(mySequence, "mySequence should not be null");

            ObjectPtr myObject = ObjectPtr(new Object());
            masl::UInt64 myDuration1 = 5;
            masl::UInt64 myDuration2 = 10;
            ObjectPropertyAnimationPtr myAnimation1 = ObjectPropertyAnimationPtr(new ObjectPropertyAnimation(myObject, &Object::setX,0,1,myDuration1));
            ObjectPropertyAnimationPtr myAnimation2 = ObjectPropertyAnimationPtr(new ObjectPropertyAnimation(myObject, &Object::setY,0,1,myDuration2));
            mySequence->add(myAnimation1);
            ENSURE_EQUAL(mySequence->getDuration(), myDuration1);
            mySequence->add(myAnimation2);
            ENSURE_EQUAL(mySequence->getDuration(), myDuration1 + myDuration2);

            //only animation1 should start playing
            AnimationManager::get().play(mySequence);
            ENSURE_EQUAL(myObject->getX(), 0);
            ENSURE_EQUAL(myObject->getY(), 0);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 1);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), true);
            ENSURE_EQUAL(myAnimation1->isRunning(), true);
            ENSURE_EQUAL(myAnimation2->isRunning(), false);

            //after duration1 animation 1 should be finished and animation 2 should start
            AnimationManager::get().doFrame(myDuration1);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 1);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), true);
            ENSURE_EQUAL(myObject->getX(), 1);
            ENSURE_EQUAL(myObject->getY(), 0);
            ENSURE_EQUAL(myAnimation1->isRunning(), false);
            ENSURE_EQUAL(myAnimation2->isRunning(), true);

            //after duration1 + duration2 all animations should be finished
            AnimationManager::get().doFrame(myDuration1 + myDuration2);
            ENSURE_EQUAL(myObject->getX(), 1);
            ENSURE_EQUAL(myObject->getY(), 1);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);
            ENSURE_EQUAL(myAnimation1->isRunning(), false);
            ENSURE_EQUAL(myAnimation2->isRunning(), false);
            ENSURE_EQUAL(mySequence->isRunning(), false);
            ENSURE_EQUAL(myAnimation1->isFinished(), true);
            ENSURE_EQUAL(myAnimation2->isFinished(), true);
            ENSURE_EQUAL(mySequence->isFinished(), true);
        }

        void perform_ParallelTest() {
            AnimationManager::get().init(0);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);

            ParallelAnimationPtr myParallel = ParallelAnimationPtr(new ParallelAnimation);
            ENSURE_MSG(myParallel, "myParallel should not be null");

            ObjectPtr myObject = ObjectPtr(new Object());
            masl::UInt64 myDuration1 = 5;
            masl::UInt64 myDuration2 = 10;
            ObjectPropertyAnimationPtr myAnimation1 = ObjectPropertyAnimationPtr(new ObjectPropertyAnimation(myObject, &Object::setX,0,1,myDuration1));
            ObjectPropertyAnimationPtr myAnimation2 = ObjectPropertyAnimationPtr(new ObjectPropertyAnimation(myObject, &Object::setY,0,1,myDuration2));
            myParallel->add(myAnimation1);
            ENSURE_EQUAL(myParallel->getDuration(), myDuration1);
            myParallel->add(myAnimation2);
            ENSURE_EQUAL(myParallel->getDuration(), std::max(myDuration1, myDuration2));

            //both animations should start playing
            AnimationManager::get().play(myParallel);
            ENSURE_EQUAL(myObject->getX(), 0);
            ENSURE_EQUAL(myObject->getY(), 0);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 1);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), true);
            ENSURE_EQUAL(myAnimation1->isRunning(), true);
            ENSURE_EQUAL(myAnimation2->isRunning(), true);

            //after duration1 animation 1 should be finished and animation2 should still be running
            AnimationManager::get().doFrame(myDuration1);
            ENSURE_EQUAL(myObject->getX(), 1);
            ENSURE_MSG(myObject->getY() > 0 && myObject->getY() < 1, "y should be between 0 and 1");
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 1);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), true);
            ENSURE_EQUAL(myAnimation1->isRunning(), false);
            ENSURE_EQUAL(myAnimation2->isRunning(), true);

            //after max(duration1, duration2) both animations should be finished
            AnimationManager::get().doFrame(std::max(myDuration1, myDuration2));
            ENSURE_EQUAL(myObject->getX(), 1);
            ENSURE_EQUAL(myObject->getY(), 1);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);
            ENSURE_EQUAL(myAnimation1->isRunning(), false);
            ENSURE_EQUAL(myAnimation2->isRunning(), false);
            ENSURE_EQUAL(myParallel->isRunning(), false);
            ENSURE_EQUAL(myAnimation1->isFinished(), true);
            ENSURE_EQUAL(myAnimation2->isFinished(), true);
            ENSURE_EQUAL(myParallel->isFinished(), true);
        }

        void perform_SequenceFinishBeforeFirstFrameTest() {
            AnimationManager::get().init(0);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
            SequenceAnimationPtr mySequence = SequenceAnimationPtr(new SequenceAnimation);
            ObjectPtr myObject = ObjectPtr(new Object());
            masl::UInt64 myDuration1 = 5;
            masl::UInt64 myDuration2 = 10;
            ObjectPropertyAnimationPtr myAnimation1 = ObjectPropertyAnimationPtr(new ObjectPropertyAnimation(myObject, &Object::setX,0,1,myDuration1));
            ObjectPropertyAnimationPtr myAnimation2 = ObjectPropertyAnimationPtr(new ObjectPropertyAnimation(myObject, &Object::setY,23,42,myDuration2));
            mySequence->add(myAnimation1);
            mySequence->add(myAnimation2);
            AnimationManager::get().play(mySequence);

            //finish before first doFrame
            mySequence->finish(0);
            ENSURE_EQUAL(mySequence->isRunning(),false);
            ENSURE_EQUAL(mySequence->isFinished(),true);
            ENSURE_EQUAL(myAnimation1->isRunning(), false);
            ENSURE_EQUAL(myAnimation1->isFinished(), true);
            ENSURE_EQUAL(myAnimation2->isRunning(), false);
            ENSURE_EQUAL(myAnimation2->isFinished(), true);
            ENSURE_EQUAL(myObject->getX(), 1);
            ENSURE_EQUAL(myObject->getY(), 42);  //y should have endValue
            AnimationManager::get().doFrame(0);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
        }

        void perform_SequenceFinishAfterFirstAnimationTest() {
            AnimationManager::get().init(0);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
            SequenceAnimationPtr mySequence = SequenceAnimationPtr(new SequenceAnimation);
            ObjectPtr myObject = ObjectPtr(new Object());
            masl::UInt64 myDuration1 = 5;
            masl::UInt64 myDuration2 = 10;
            ObjectPropertyAnimationPtr myAnimation1 = ObjectPropertyAnimationPtr(new ObjectPropertyAnimation(myObject, &Object::setX,0,1,myDuration1));
            ObjectPropertyAnimationPtr myAnimation2 = ObjectPropertyAnimationPtr(new ObjectPropertyAnimation(myObject, &Object::setY,23,42,myDuration2));
            mySequence->add(myAnimation1);
            mySequence->add(myAnimation2);
            AnimationManager::get().play(mySequence);
            AnimationManager::get().doFrame(myDuration1);

            //finish after first Animation
            mySequence->finish(myDuration1);
            ENSURE_EQUAL(mySequence->isRunning(),false);
            ENSURE_EQUAL(mySequence->isFinished(),true);
            ENSURE_EQUAL(myAnimation1->isRunning(), false);
            ENSURE_EQUAL(myAnimation1->isFinished(), true);
            ENSURE_EQUAL(myAnimation2->isRunning(), false);
            ENSURE_EQUAL(myAnimation2->isFinished(), true);
            ENSURE_EQUAL(myObject->getX(), 1);
            ENSURE_EQUAL(myObject->getY(), 42);  //y should have endValue
            AnimationManager::get().doFrame(myDuration1);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
        }

        void perform_SequenceCancelTest() {
            AnimationManager::get().init(0);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
            SequenceAnimationPtr mySequence = SequenceAnimationPtr(new SequenceAnimation);
            ObjectPtr myObject = ObjectPtr(new Object());
            masl::UInt64 myDuration1 = 5;
            masl::UInt64 myDuration2 = 10;
            ObjectPropertyAnimationPtr myAnimation1 = ObjectPropertyAnimationPtr(new ObjectPropertyAnimation(myObject, &Object::setX,0,1,myDuration1));
            ObjectPropertyAnimationPtr myAnimation2 = ObjectPropertyAnimationPtr(new ObjectPropertyAnimation(myObject, &Object::setY,23,42,myDuration2));
            mySequence->add(myAnimation1);
            mySequence->add(myAnimation2);
            AnimationManager::get().play(mySequence);
            AnimationManager::get().doFrame(myDuration1-2);

            //cancel before first animation finished
            mySequence->cancel();
            ENSURE_EQUAL(mySequence->isRunning(),false);
            ENSURE_EQUAL(mySequence->isFinished(),false);
            ENSURE_EQUAL(myAnimation1->isRunning(), false);
            ENSURE_EQUAL(myAnimation1->isFinished(), false);
            ENSURE_EQUAL(myAnimation2->isRunning(), false);
            ENSURE_EQUAL(myAnimation2->isFinished(), false);
            ENSURE_MSG(myObject->getX() > 0 && myObject->getX() < 1 , "x should be between 0 and 1");
            //ENSURE_EQUAL(myObject->getY(), 23);   //this can not be guaranteed when canceling, y==0
            AnimationManager::get().doFrame(myDuration1-1);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
        }

        void perform_ParallelFinishTest() {
            AnimationManager::get().init(0);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
            ParallelAnimationPtr myParallel = ParallelAnimationPtr(new ParallelAnimation);
            ObjectPtr myObject = ObjectPtr(new Object());
            masl::UInt64 myDuration1 = 5;
            masl::UInt64 myDuration2 = 10;
            ObjectPropertyAnimationPtr myAnimation1 = ObjectPropertyAnimationPtr(new ObjectPropertyAnimation(myObject, &Object::setX,0,1,myDuration1));
            ObjectPropertyAnimationPtr myAnimation2 = ObjectPropertyAnimationPtr(new ObjectPropertyAnimation(myObject, &Object::setY,23,42,myDuration2));
            myParallel->add(myAnimation1);
            myParallel->add(myAnimation2);
            AnimationManager::get().play(myParallel);

            //finish before first animation finished
            myParallel->finish(myDuration1-2);
            ENSURE_EQUAL(myParallel->isRunning(),false);
            ENSURE_EQUAL(myParallel->isFinished(),true);
            ENSURE_EQUAL(myAnimation1->isRunning(), false);
            ENSURE_EQUAL(myAnimation1->isFinished(), true);
            ENSURE_EQUAL(myAnimation2->isRunning(), false);
            ENSURE_EQUAL(myAnimation2->isFinished(), true);
            ENSURE_EQUAL(myObject->getX(), 1);
            ENSURE_EQUAL(myObject->getY(), 42);   //y should be at end value
            AnimationManager::get().doFrame(myDuration1-1);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
        }

        void perform_parallelCancelTest() {
            AnimationManager::get().init(0);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
            ParallelAnimationPtr myParallel = ParallelAnimationPtr(new ParallelAnimation);
            ObjectPtr myObject = ObjectPtr(new Object());
            masl::UInt64 myDuration1 = 5;
            masl::UInt64 myDuration2 = 10;
            ObjectPropertyAnimationPtr myAnimation1 = ObjectPropertyAnimationPtr(new ObjectPropertyAnimation(myObject, &Object::setX,0,1,myDuration1));
            ObjectPropertyAnimationPtr myAnimation2 = ObjectPropertyAnimationPtr(new ObjectPropertyAnimation(myObject, &Object::setY,23,42,myDuration2));
            myParallel->add(myAnimation1);
            myParallel->add(myAnimation2);
            AnimationManager::get().play(myParallel);

            //cancel before first animation finished
            AnimationManager::get().doFrame(myDuration1-2);
            myParallel->cancel();
            ENSURE_EQUAL(myParallel->isRunning(),false);
            ENSURE_EQUAL(myParallel->isFinished(),false);
            ENSURE_EQUAL(myAnimation1->isRunning(), false);
            ENSURE_EQUAL(myAnimation1->isFinished(), false);
            ENSURE_EQUAL(myAnimation2->isRunning(), false);
            ENSURE_EQUAL(myAnimation2->isFinished(), false);
            ENSURE_MSG(myObject->getX() > 0 && myObject->getX() < 1, "x should be between 0 and 1");
            ENSURE_MSG(myObject->getY() > 23 && myObject->getY() < 42, "y should be between 23 and 42");
            AnimationManager::get().doFrame(myDuration1-1);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
        }
    };
};
#endif
