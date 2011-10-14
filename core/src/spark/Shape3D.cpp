#include "Shape3D.h"

#include <mar/ObjImporter.h>
#include "BaseApp.h"
#include "SparkComponentFactory.h"

namespace spark {
    const char * const Shape3D::SPARK_TYPE = "Shape3D";

    Shape3D::Shape3D(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        ShapeWidget(theApp, theXMLNode),
        _mySrc(_myXMLNode->getAttributeAs<std::string>("src", ""))
    {}

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
        _myShape = mar::ShapePtr(new mar::ObjShape());
        mar::ObjImporter::get().importObj(_mySrc, _myShape);
    }

}
