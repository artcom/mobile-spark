#ifndef _included_mobile_spark_Transform_
#define _included_mobile_spark_Transform_

#include "Widget.h"

namespace spark {
    ComponentPtr createTransform(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());

    static const char* TRANSFORM_TYPE = "Transform";
    class Transform : public Widget {
    public: 
        Transform(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~Transform();
    };

    typedef boost::shared_ptr<Transform> TransformPtr;
};

#endif 
