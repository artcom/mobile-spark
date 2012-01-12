// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_animation_PropertyAnimation_h_included_
#define _ac_mobile_animation_PropertyAnimation_h_included_

#include <masl/Logger.h>
#include "Animation.h"


namespace animation {

    template <class O, class T>
    class PropertyAnimation : public Animation {
    public:
        PropertyAnimation(const O theObject, const T thePropertySetter, const float theStartValue = 0.0f,
                          const float theEndValue = 1.0f, const masl::UInt64 theDuration = 1000, const EasingFunctionPtr theEasing = defaultEasing);
        virtual ~PropertyAnimation();
        virtual void doFrame(const masl::UInt64 theTime);

        virtual void play(const masl::UInt64 theTime, const bool theComeToAnEndFlag = false);
        virtual void finish(const masl::UInt64 theTime);

    private:
        const O _myObjectPtr;  //should be WeakPtr
        const T _myPropertyCallback;
        const float _myStartValue;
        const float _myEndValue;
    };


    /////////////////////////////////////////implementation
    //(has to be in header due to template)
    template <class O, class T>
    PropertyAnimation<O, T>::PropertyAnimation(
        const O theObjectPtr,
        const T thePropertySetter,
        const float theStartValue,
        const float theEndValue,
        const masl::UInt64 theDuration,
        const EasingFunctionPtr theEasing):
            Animation(theDuration, theEasing),
            _myObjectPtr(theObjectPtr),
            _myPropertyCallback(thePropertySetter),
            _myStartValue(theStartValue),
            _myEndValue(theEndValue) {
    }

    template <class O, class T>
    PropertyAnimation<O, T>::~PropertyAnimation(){}

    template <class O,class T>
    void PropertyAnimation<O, T>::doFrame(const masl::UInt64 theCurrentMillis) {
        Animation::doFrame(theCurrentMillis);
        float value = _myStartValue + (_myEndValue - _myStartValue) * _myProgress;
        AC_TRACE << "progress " << _myProgress << " value: " << value;
        if (_myObjectPtr.lock()) {
            (_myObjectPtr.lock().get()->*_myPropertyCallback)(value);
        }
    }

    template <class O, class T>
    void PropertyAnimation<O, T>::play(const masl::UInt64 theTime, const bool theComeToAnEndFlag) {
        AC_DEBUG << "property animation set value to start value " << _myStartValue;
        if (_myObjectPtr.lock()) {
            (_myObjectPtr.lock().get()->*_myPropertyCallback)(_myStartValue);
        }
        Animation::play(theTime, theComeToAnEndFlag);
    }

    template <class O, class T>
    void PropertyAnimation<O, T>::finish(const masl::UInt64 theTime) {
        if (_myObjectPtr.lock()) {
            (_myObjectPtr.lock().get()->*_myPropertyCallback)(_myEndValue);
        }
        Animation::finish(theTime);
    }
};

#endif

