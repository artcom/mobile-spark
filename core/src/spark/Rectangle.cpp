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
        setVertexData(element, width, height);
        _myShape->elementList.push_back(element);
        UnlitColoredMaterialPtr myMaterial = UnlitColoredMaterialPtr(new UnlitColoredMaterial(_myApp->assetProvider));
        myMaterial->diffuse = _myColor;
        element->material = myMaterial;
        element->material->createShader();
    }

    Rectangle::~Rectangle() {
    }

    void Rectangle::renderShape() const {
        _myApp->window->renderShape(_myShape);
    }

    void Rectangle::setVertexData(ElementPtr theElement, const float theWidth, const float theHeight) {
        (theElement->vertexData)[0] = 0.0f;
        (theElement->vertexData)[1] = 0.0f;
        (theElement->vertexData)[2] = 0.0f;
        (theElement->vertexData)[3] = theWidth;
        (theElement->vertexData)[4] = 0.0f;
        (theElement->vertexData)[5] = 0.0f;
        (theElement->vertexData)[6] = 0.0f;
        (theElement->vertexData)[7] = theHeight;
        (theElement->vertexData)[8] = 0.0f;
        (theElement->vertexData)[9] = theWidth;
        (theElement->vertexData)[10] = 0.0f;
        (theElement->vertexData)[11] = 0.0f;
        (theElement->vertexData)[12] = theWidth;
        (theElement->vertexData)[13] = theHeight;
        (theElement->vertexData)[14] = 0.0f;
        (theElement->vertexData)[15] = 0.0f;
        (theElement->vertexData)[16] = theHeight;
        (theElement->vertexData)[17] = 0.0f;
    }
}
