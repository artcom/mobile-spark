#include "Rectangle.h"

#include <mar/Shape.h>
#include <mar/Material.h>
#include "SparkComponentFactory.h"

namespace spark {

    const char * const Rectangle::SPARK_TYPE = "Rectangle";

    Rectangle::Rectangle(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        ShapeWidget(theApp, theXMLNode),
        _myColor(_myXMLNode->getAttributeAs<vector3>("color", vector3(1,1,1)))
    {
        mar::UnlitColoredMaterialPtr myMaterial = mar::UnlitColoredMaterialPtr(new mar::UnlitColoredMaterial());
        myMaterial->setDiffuseColor(_myColor);
        _myShape = mar::ShapePtr(new mar::RectangleShape(myMaterial));
        setSize(vector2(getNode()->getAttributeAs<float>("width"), getNode()->getAttributeAs<float>("height")));
    }

    Rectangle::~Rectangle() {
    }
    
}
