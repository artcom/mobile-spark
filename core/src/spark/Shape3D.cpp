#include "Shape3D.h"

#include "BaseApp.h"
#include "SparkComponentFactory.h"

namespace spark {
    //needed for component factory
    //namespace  {
        ComponentPtr createShape3D(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent) {
            return Shape3DPtr(new Shape3D(theApp, theXMLNode, theParent));
        };
    //    const bool registered = spark::SparkComponentFactory::get().registerComponent("Image", spark::createImage);
    //}


    Shape3D::Shape3D(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        ShapeWidget(theApp, theXMLNode, theParent) {

        _mySrc = _myXMLNode->getStringValue("src");
        _myShape = ShapeFactory::get().createObj(_mySrc);
    }

    Shape3D::~Shape3D() {
    }
}
