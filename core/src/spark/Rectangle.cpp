#include "Rectangle.h"

#include "SparkComponentFactory.h"

namespace spark {

    const char * const Rectangle::SPARK_TYPE = "Rectangle";

    Rectangle::Rectangle(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        ShapeWidget(theApp, theXMLNode),
        _myColor(_myXMLNode->getAttributeAs<vector3>("color", vector3(1,1,1)))
    {
        float myWidth = getNode()->getAttributeAs<float>("width");
        float myHeight = getNode()->getAttributeAs<float>("height");
        setShape(mar::ShapeFactory::get().createRectangle(false, myWidth, myHeight));

        boost::static_pointer_cast<mar::UnlitColoredMaterial>(getShape()->elementList[0]->material)->setDiffuseColor(_myColor);
    }

    Rectangle::~Rectangle() {
    }
    
}
