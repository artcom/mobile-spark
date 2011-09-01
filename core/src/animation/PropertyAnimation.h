#ifndef _ac_mobile_animation_PropertyAnimation_h_included_
#define _ac_mobile_animation_PropertyAnimation_h_included_

#include <masl/Logger.h>
#include "Animation.h"


namespace animation {

    template <class O, class T>
    class PropertyAnimation : public Animation {
    public:
        PropertyAnimation(const O theObject, const T thePropertySetter, const float theStartValue = 0.0f, const float theEndValue = 1.0f, const masl::UInt64 theDuration = 1000, const EasingFunctionPtr theEasing = defaultEasing);
        virtual ~PropertyAnimation();
        virtual void doFrame(const masl::UInt64 theTime);

        virtual void play(const masl::UInt64 theTime, const bool theComeToAnEndFlag = false);
        virtual void finish(const masl::UInt64 theTime);

    private:
        const O _myObjectPtr;
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
        //AC_PRINT << "progress " << _myProgress;
        float value = _myStartValue + (_myEndValue - _myStartValue) * _myProgress;
        //AC_PRINT << "value "  << value;
        (_myObjectPtr.get()->*_myPropertyCallback)(value);
    }

    template <class O, class T>
    void PropertyAnimation<O, T>::play(const masl::UInt64 theTime, const bool theComeToAnEndFlag) {
        Animation::play(theTime, theComeToAnEndFlag);
        //AC_PRINT << "property animation set value to start value " << _myStartValue;
        (_myObjectPtr.get()->*_myPropertyCallback)(_myStartValue);
    }

    template <class O, class T>
    void PropertyAnimation<O, T>::finish(const masl::UInt64 theTime) {
        Animation::finish(theTime);
        (_myObjectPtr.get()->*_myPropertyCallback)(_myEndValue);
    }
};

#endif

