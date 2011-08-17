#include "PropertyAnimation.h"

#include <masl/Logger.h>

namespace animation {

    PropertyAnimation::PropertyAnimation(
        const PropertySetterFunction thePropertySetter, 
        const float theStartValue, 
        const float theEndValue, 
        const long theDuration): 
            Animation(theDuration),
            _myPropertyCallback(thePropertySetter),
            _myStartValue(theStartValue),
            _myEndValue(theEndValue) {
    }

    void PropertyAnimation::doFrame(const long theCurrentMillis) {
        Animation::doFrame(theCurrentMillis);
        //AC_PRINT << "progress " << _myProgress;
        float value = _myStartValue + (_myEndValue - _myStartValue) * _myProgress;
        //AC_PRINT << "value "  << value;
        _myPropertyCallback(value);
    }
}
