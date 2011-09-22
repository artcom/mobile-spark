#include "Shape3D.h"

#include "BaseApp.h"
#include "SparkComponentFactory.h"

namespace spark {
    const char * const Shape3D::SPARK_TYPE = "Shape3D";

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
        _myDirtyFlag = true;
    }

    void
    Shape3D::build() {
        ShapeWidget::build();
        setShape(ShapeFactory::get().createObj(_mySrc));
    }

}
