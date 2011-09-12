#include "Rectangle.h"
#include "BaseApp.h"

#include "SparkComponentFactory.h"

namespace spark {
    //needed for component factory
    namespace  {
        const bool registered = spark::SparkComponentFactory::get().registerComponent("Rectangle", spark::create<Rectangle>);
    }


    Rectangle::Rectangle(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        ShapeWidget(theApp, theXMLNode, theParent) {

        float width = _myXMLNode->getAttributeAs<float>("width");
        float height = _myXMLNode->getAttributeAs<float>("height");
        vector3 myColor = _myXMLNode->getAttributeAs<vector3>("color", vector3(1,1,1));
        _myColor[0] = myColor[0];
        _myColor[1] = myColor[1];
        _myColor[2] = myColor[2];
        _myColor[3] = 1; //TODO: use alpha

        setShape(ShapeFactory::get().createRectangle(false, width, height));

        //XXX
        boost::static_pointer_cast<UnlitColoredMaterial>(getShape()->elementList[0]->material)->setDiffuseColor(_myColor);
    }

    Rectangle::~Rectangle() {
    }
    
}
