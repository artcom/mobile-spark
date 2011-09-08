#include "Transform.h"

#include "SparkComponentFactory.h"


namespace spark {
    const char * Transform::SPARK_TYPE = "Transform";

    //needed for component factory
    //namespace  {
        ComponentPtr createTransform(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent) {
            return TransformPtr(new Transform(theApp, theXMLNode, theParent));
        };
        //const bool registered = spark::SparkComponentFactory::get().registerComponent("Transform", spark::createTransform);
    //}


    Transform::Transform(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        Widget(theApp, theXMLNode, theParent) {
    }

    Transform::~Transform() {
    }

}
