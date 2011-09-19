#include "Rectangle.h"

#include "SparkComponentFactory.h"

namespace spark {
    
    const char * const Rectangle::SPARK_TYPE = "Rectangle";

    Rectangle::Rectangle(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        ShapeWidget(theApp, theXMLNode, theParent) {

        float width = _myXMLNode->getAttributeAs<float>("width");
        float height = _myXMLNode->getAttributeAs<float>("height");
        vector3 myColor = _myXMLNode->getAttributeAs<vector3>("color", vector3(1,1,1));
        _myColor[0] = myColor[0];
        _myColor[1] = myColor[1];
        _myColor[2] = myColor[2];
        _myColor[3] = 1; //TODO: use alpha

        setShape(mar::ShapeFactory::get().createRectangle(false, width, height));

        //XXX
        boost::static_pointer_cast<mar::UnlitColoredMaterial>(getShape()->elementList[0]->material)->setDiffuseColor(_myColor);
    }

    Rectangle::~Rectangle() {
    }
    
}
