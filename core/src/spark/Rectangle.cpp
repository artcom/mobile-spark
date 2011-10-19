#include "Rectangle.h"

#include "SparkComponentFactory.h"

namespace spark {

    const char * const Rectangle::SPARK_TYPE = "Rectangle";

    Rectangle::Rectangle(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        ShapeWidget(theApp, theXMLNode),
        _myColor(_myXMLNode->getAttributeAs<vector3>("color", vector3(1,1,1)))
    {
        setShape(mar::ShapeFactory::get().createRectangle(false, 1, 1));
        setSize(vector2(getNode()->getAttributeAs<float>("width"), getNode()->getAttributeAs<float>("height")));
        boost::static_pointer_cast<mar::UnlitColoredMaterial>(getShape()->elementList_[0]->material)->setDiffuseColor(_myColor);
    }

    Rectangle::~Rectangle() {
    }
    
    std::string 
    Rectangle::getAttributesAsString() const {
        return ShapeWidget::getAttributesAsString() + " color=\""+masl::as_string(_myColor)+"\"";
    }

}
