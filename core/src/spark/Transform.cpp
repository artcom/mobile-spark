#include "Transform.h"

#include "SparkComponentFactory.h"


namespace spark {
    const char * const Transform::SPARK_TYPE = "Transform";

    Transform::Transform(const BaseAppPtr theApp, const XMLNodePtr theXMLNode):
        Widget(theApp, theXMLNode) {
    }

    Transform::~Transform() {
    }

}
