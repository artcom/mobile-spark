#include "Rectangle.h"
#include "BaseApp.h"

#include "SparkComponentFactory.h"

namespace spark {
    //needed for component factory
    namespace  {
        ComponentPtr createRectangle(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr()) {
            return RectanglePtr(new Rectangle(theApp, theXMLNode, theParent));
        };
        const bool registered = spark::SparkComponentFactory::get().registerComponent("Rectangle", spark::createRectangle);
    }


    Rectangle::Rectangle(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        Widget(theApp, theXMLNode, theParent) {

        float width = _myXMLNode->getFloatValue("width");
        float height = _myXMLNode->getFloatValue("height");
        _myColor = _myXMLNode->getFloatArrayValue("color");
        _myColor = _myColor.size() > 2 ? _myColor : std::vector<float>(4,1.0f);
        if (_myColor.size() == 3) {
            _myColor.push_back(1.0f);
        }

        _myShape = ShapePtr(new Shape());
        ElementPtr element = ElementPtr(new Element());
        size_t dataPerVertex = 3;
        element->numVertices = 6;
        element->vertexData = boost::shared_array<float>(new float[(element->numVertices) * dataPerVertex]);
        (element->vertexData)[0] = 0.0f;
        (element->vertexData)[1] = 0.0f;
        (element->vertexData)[2] = 0.0f;
        (element->vertexData)[3] = width;
        (element->vertexData)[4] = 0.0f;
        (element->vertexData)[5] = 0.0f;
        (element->vertexData)[6] = 0.0f;
        (element->vertexData)[7] = height;
        (element->vertexData)[8] = 0.0f;
        (element->vertexData)[9] = width;
        (element->vertexData)[10] = 0.0f;
        (element->vertexData)[11] = 0.0f;
        (element->vertexData)[12] = width;
        (element->vertexData)[13] = height;
        (element->vertexData)[14] = 0.0f;
        (element->vertexData)[15] = 0.0f;
        (element->vertexData)[16] = height;
        (element->vertexData)[17] = 0.0f;
        _myShape->elementList.push_back(element);

        element->material = MaterialPtr(new Material(_myApp->assetProvider));
        element->material->diffuse = _myColor;
        element->material->createShader();
 
        //_myShape->materialMap["xxx"] = element->material; //XXX: needed?
    }

    Rectangle::~Rectangle() {
    }

    void Rectangle::drawObject() const {
        _myApp->window->drawObject(_myShape);
    }

    ComponentPtr createRectangle(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent) {
        return RectanglePtr(new Rectangle(theApp, theXMLNode, theParent));
    };
}
