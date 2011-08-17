#ifndef _ac_mobile_animation_PropertyAnimation_h_included_
#define _ac_mobile_animation_PropertyAnimation_h_included_

#include <masl/Logger.h>
#include "Animation.h"


namespace animation {

    template <class O, class T>
    class PropertyAnimation : public Animation {
    public:
        PropertyAnimation(const O theObject, const T thePropertySetter, const float theStartValue = 0.0f, const float theEndValue = 1.0f, const long theDuration = 1000);
        virtual void doFrame(const long theTime);

    private:
        const O _myObject;
        const T _myPropertyCallback;
        const float _myStartValue;
        const float _myEndValue;
    };

    template <class O, class T>
    PropertyAnimation<O, T>::PropertyAnimation(
        const O theObject,
        const T thePropertySetter, 
        const float theStartValue, 
        const float theEndValue, 
        const long theDuration): 
            Animation(theDuration),
            _myObject(theObject),
            _myPropertyCallback(thePropertySetter),
            _myStartValue(theStartValue),
            _myEndValue(theEndValue) {
    }

    template <class O,class T>
    void PropertyAnimation<O, T>::doFrame(const long theCurrentMillis) {
        Animation::doFrame(theCurrentMillis);
        //AC_PRINT << "progress " << _myProgress;
        float value = _myStartValue + (_myEndValue - _myStartValue) * _myProgress;
        //AC_PRINT << "value "  << value;
        (_myObject.get()->*_myPropertyCallback)(value);
    }
};

#endif

