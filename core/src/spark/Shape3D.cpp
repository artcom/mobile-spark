#include "Shape3D.h"

#include "BaseApp.h"
#include "SparkComponentFactory.h"

namespace spark {
    //needed for component factory
    namespace  {
        const bool registered = spark::SparkComponentFactory::get().registerComponent("Shape3D", spark::create<Shape3D>);
    }


    Shape3D::Shape3D(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        ShapeWidget(theApp, theXMLNode, theParent) {

        _mySrc = _myXMLNode->getAttributeAs<std::string>("src", "");
        setShape(ShapeFactory::get().createObj(_mySrc));
    }

    Shape3D::~Shape3D() {
    }

    void 
    Shape3D::onResume() {        
        ShapeWidget::onResume();        
        setShape(ShapeFactory::get().createObj(_mySrc));
    }

}
