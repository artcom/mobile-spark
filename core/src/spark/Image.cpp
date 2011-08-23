#include "Image.h"

#include <android/AndroidAssetProvider.h>
#include <mar/png_functions.h>
#include "BaseApp.h"
#include "SparkComponentFactory.h"

namespace spark {
    //needed for component factory
    //namespace  {
        ComponentPtr createImage(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent) {
            return ImagePtr(new Image(theApp, theXMLNode, theParent));
        };
    //    const bool registered = spark::SparkComponentFactory::get().registerComponent("Image", spark::createImage);
    //}


    Image::Image(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        Widget(theApp, theXMLNode, theParent) {
        _mySrc = _myXMLNode->getStringValue("src");
        _myShape = ShapePtr(new Shape());
        ElementPtr element = ElementPtr(new ElementWithTexture());
        size_t dataPerVertex = 3 + 2;
        element->numVertices = 6;
        element->vertexData = boost::shared_array<float>(new float[(element->numVertices) * dataPerVertex]);
        //TODO: material inheritance?
        element->material = MaterialPtr(new Material(_myApp->assetProvider, UNLIT_TEXTURED_MATERIAL));
        loadTextureFromPNG(theApp->assetProvider, _mySrc, element->material);
        float width = _myXMLNode->getFloatValue("width", element->material->width);
        float height = _myXMLNode->getFloatValue("height", element->material->height);
        setVertexData(element, width, height);
        _myShape->elementList.push_back(element);
        //material is ready -> create shader now
        element->material->createShader();
    }

    Image::~Image() {
    }

    void Image::renderShape() const {
        _myApp->window->renderShape(_myShape);
    }

    void Image::setVertexData(ElementPtr theElement, const float theWidth, const float theHeight) {
        (theElement->vertexData)[0] = 0.0f;
        (theElement->vertexData)[1] = 0.0f;
        (theElement->vertexData)[2] = 0.0f;
        (theElement->vertexData)[3] = 0.0f;
        (theElement->vertexData)[4] = 0.0f;

        (theElement->vertexData)[5] = theWidth;
        (theElement->vertexData)[6] = 0.0f;
        (theElement->vertexData)[7] = 0.0f;
        (theElement->vertexData)[8] = 1.0f;
        (theElement->vertexData)[9] = 0.0f;

        (theElement->vertexData)[10] = 0.0f;
        (theElement->vertexData)[11] = theHeight;
        (theElement->vertexData)[12] = 0.0f;
        (theElement->vertexData)[13] = 0.0f;
        (theElement->vertexData)[14] = 1.0f;

        (theElement->vertexData)[15] = theWidth;
        (theElement->vertexData)[16] = 0.0f;
        (theElement->vertexData)[17] = 0.0f;
        (theElement->vertexData)[18] = 1.0f;
        (theElement->vertexData)[19] = 0.0f;

        (theElement->vertexData)[20] = theWidth;
        (theElement->vertexData)[21] = theHeight;
        (theElement->vertexData)[22] = 0.0f;
        (theElement->vertexData)[23] = 1.0f;
        (theElement->vertexData)[24] = 1.0f;

        (theElement->vertexData)[25] = 0.0f;
        (theElement->vertexData)[26] = theHeight;
        (theElement->vertexData)[27] = 0.0f;
        (theElement->vertexData)[28] = 0.0f;
        (theElement->vertexData)[29] = 1.0f;
    }
}
