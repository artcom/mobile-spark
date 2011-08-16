#ifndef _ac_mobile_animation_PropertyAnimation_h_included_
#define _ac_mobile_animation_PropertyAnimation_h_included_

#include "Animation.h"

namespace animation {

    typedef void (*PropertySetterFunction)(float);
    class PropertyAnimation : public Animation {
    public:
        PropertyAnimation(const PropertySetterFunction thePropertySetter, const float theStartValue = 0.0f, const float theEndValue = 1.0f, const long theDuration = 1000);
    };

    typedef boost::shared_ptr<PropertyAnimation> PropertyAnimationPtr;
};

#endif
