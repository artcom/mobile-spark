#include "Transform.h"

#include "SparkComponentFactory.h"


namespace spark {
    Transform::Transform(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        Widget(theApp, theXMLNode, theParent) {
        SparkComponentFactory::get().registerComponent(std::string("Transform"), createTransform);
    }

    Transform::~Transform() {
    }

    ComponentPtr createTransform(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent) {
        return TransformPtr(new Transform(theApp, theXMLNode, theParent));
    };
}
