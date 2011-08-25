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

        float width = _myXMLNode->getFloatValue("width");
        float height = _myXMLNode->getFloatValue("height");
        _myColor = _myXMLNode->getFloatArrayValue("color");
        _myColor = _myColor.size() > 2 ? _myColor : std::vector<float>(4,1.0f);
        if (_myColor.size() == 3) {
            _myColor.push_back(1.0f);
        }

        _myShape = ShapeFactory::get().createRectangle(false);

        //XXX
        boost::static_pointer_cast<UnlitColoredMaterial>(_myShape->elementList[0]->material)->diffuse = _myColor;
        _myShape->setDimensions(width, height);
    }

    Rectangle::~Rectangle() {
    }
}
