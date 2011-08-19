/* __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2008, ART+COM AG Berlin, Germany <www.artcom.de>
//
// These coded instructions, statements, and computer programs contain
// proprietary information of ART+COM AG Berlin, and are copy protected
// by law. They may be used, modified and redistributed under the terms
// of GNU General Public License referenced below.
//
// Alternative licensing without the obligations of the GPL is
// available upon request.
//
// GPL v3 Licensing:
//
// This file is part of the ART+COM Y60 Platform.
//
// ART+COM Y60 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// ART+COM Y60 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with ART+COM Y60.  If not, see <http://www.gnu.org/licenses/>.
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
*/
#ifndef _ac_mobile_asl_test_CompositeAnimation_h_included_
#define _ac_mobile_asl_test_CompositeAnimation_h_included_

#include <string>
#include <algorithm>
#include <boost/smart_ptr/shared_ptr.hpp>
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
        typedef boost::shared_ptr<Object> ObjectPtr;

        //animations
        typedef void (Object::* ObjectPropertySetterFunction)(float);
        typedef PropertyAnimation<ObjectPtr, ObjectPropertySetterFunction> ObjectPropertyAnimation;
        typedef boost::shared_ptr<ObjectPropertyAnimation>  ObjectPropertyAnimationPtr;

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
            long myDuration1 = 5;
            long myDuration2 = 10;
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
            long myDuration1 = 5;
            long myDuration2 = 10;
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
            long myDuration1 = 5;
            long myDuration2 = 10;
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
            long myDuration1 = 5;
            long myDuration2 = 10;
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
            long myDuration1 = 5;
            long myDuration2 = 10;
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
            //ENSURE_EQUAL(myObject->getY(), 23);   //this can not be guaranteed when canceling
            AnimationManager::get().doFrame(myDuration1-1);
            ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);
            ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
        }
            
        void perform_ParallelFinishTest() {
        }

        void perform_parallelCancelTest() {
        }
    };    
};
#endif
