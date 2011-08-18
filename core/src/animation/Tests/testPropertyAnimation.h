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
#ifndef _ac_mobile_asl_test_PropertyAnimation_h_included_
#define _ac_mobile_asl_test_PropertyAnimation_h_included_

#include <string>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <masl/UnitTest.h>

#include "../AnimationManager.h"
#include "../PropertyAnimation.h"


namespace animation {
    class PropertyAnimation_UnitTest : public UnitTest {
        public:
            PropertyAnimation_UnitTest() : UnitTest("PropertyAnimation_UnitTest") {  }
            void run() {
                perform_AnimationManagerTest();
                perform_PropertyAnimationTest();
                perform_LoopAnimationTest();
            }
            
            class Object {
                private: 
                    float x;
                public: 
                    Object() { x = 0; };
                    void setX(float theX) { x = theX;};
                    float getX() { return x;};
            };
            typedef boost::shared_ptr<Object> ObjectPtr;

            //animations
            typedef void (Object::* ObjectPropertySetterFunction)(float);
            typedef PropertyAnimation<ObjectPtr, ObjectPropertySetterFunction> ObjectPropertyAnimation;
            typedef boost::shared_ptr<ObjectPropertyAnimation>  ObjectPropertyAnimationPtr;

            void perform_AnimationManagerTest() {
                ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
                ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);
                AnimationManager::get().doFrame(0);
                ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
                ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);
                AnimationManager::get().doFrame(1);
                ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
                ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);
            }

            void perform_PropertyAnimationTest() {                
                ObjectPtr myObject = ObjectPtr(new Object());
                ENSURE_MSG(myObject, "myObject should not be null");
                ENSURE_EQUAL(myObject->getX(),0);

                AnimationManager::get().doFrame(0);
                ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
                ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);

                ObjectPropertyAnimationPtr myAnimation = ObjectPropertyAnimationPtr(new ObjectPropertyAnimation(myObject, &Object::setX));
                ENSURE_MSG(myAnimation, "myAnimation should not be null");
                ENSURE_EQUAL(myAnimation->isRunning(),false);
                ENSURE_EQUAL(myAnimation->isFinished(),false);
                AnimationManager::get().play(myAnimation);
                ENSURE_EQUAL(myObject->getX(),0);
                ENSURE_EQUAL(myAnimation->isRunning(),true);
                ENSURE_EQUAL(myAnimation->isFinished(),false);
                ENSURE_EQUAL(AnimationManager::get().animationCount(), 1);
                ENSURE_EQUAL(AnimationManager::get().isPlaying(), true);

                AnimationManager::get().doFrame(999);
                ENSURE_EQUAL(AnimationManager::get().animationCount(), 1);
                ENSURE_EQUAL(AnimationManager::get().isPlaying(), true);
                ENSURE_EQUAL(myAnimation->isRunning(),true);
                ENSURE_EQUAL(myAnimation->isFinished(),false);

                AnimationManager::get().doFrame(1000);
                ENSURE_EQUAL(myAnimation->isRunning(),false);
                ENSURE_EQUAL(myAnimation->isFinished(),true);
                ENSURE_EQUAL(AnimationManager::get().animationCount(), 0);
                ENSURE_EQUAL(AnimationManager::get().isPlaying(), false);
                ENSURE_EQUAL(myObject->getX(),1);
            }

            void perform_LoopAnimationTest() {
                ObjectPtr myObject = ObjectPtr(new Object());
                ObjectPropertyAnimationPtr myAnimation = ObjectPropertyAnimationPtr(new ObjectPropertyAnimation(myObject, &Object::setX));
                myAnimation->setLoop(true);
                ENSURE_MSG(myAnimation, "myAnimation should not be null");
                ENSURE_EQUAL(myAnimation->isRunning(),false);
                ENSURE_EQUAL(myAnimation->isFinished(),false);
                AnimationManager::get().play(myAnimation);
                ENSURE_EQUAL(myObject->getX(),0);
                ENSURE_EQUAL(myAnimation->isRunning(),true);
                ENSURE_EQUAL(myAnimation->isFinished(),false);
                ENSURE_EQUAL(AnimationManager::get().animationCount(), 1);
                ENSURE_EQUAL(AnimationManager::get().isPlaying(), true);

                AnimationManager::get().doFrame(1000);
                ENSURE_EQUAL(myAnimation->isRunning(),true);
                ENSURE_EQUAL(myAnimation->isFinished(),false);
                ENSURE_EQUAL(AnimationManager::get().animationCount(), 1);
                ENSURE_EQUAL(AnimationManager::get().isPlaying(), true);
                ENSURE_EQUAL(myObject->getX(),0);
            }
    };    
};
#endif
