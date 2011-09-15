#include "Transform.h"

#include "SparkComponentFactory.h"


namespace spark {
    const char * Transform::SPARK_TYPE = "Transform";

    Transform::Transform(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        Widget(theApp, theXMLNode, theParent) {
    }

    Transform::~Transform() {
    }

}
