#include "Rectangle.h"
#include "BaseApp.h"

#include "SparkComponentFactory.h"

namespace spark {
    //needed for component factory
    //namespace  {
        ComponentPtr createRectangle(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent) {
            return RectanglePtr(new Rectangle(theApp, theXMLNode, theParent));
        };
    //    const bool registered = spark::SparkComponentFactory::get().registerComponent("Rectangle", spark::createRectangle);
    //}


    Rectangle::Rectangle(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        ShapeWidget(theApp, theXMLNode, theParent) {

        float width = _myXMLNode->getAttributeAs<float>("width");
        float height = _myXMLNode->getAttributeAs<float>("height");
        _myColor = _myXMLNode->getFloatArrayValue("color");
        _myColor = _myColor.size() > 2 ? _myColor : std::vector<float>(4,1.0f);
        if (_myColor.size() == 3) {
            _myColor.push_back(1.0f);
        }

        setShape(ShapeFactory::get().createRectangle(false));

        //XXX
        boost::static_pointer_cast<UnlitColoredMaterial>(getShape()->elementList[0]->material)->diffuse = _myColor;
        getShape()->setDimensions(width, height);
    }

    Rectangle::~Rectangle() {
    }
    
}
