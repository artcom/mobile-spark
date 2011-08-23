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

        float width = _myXMLNode->getFloatValue("width");
        float height = _myXMLNode->getFloatValue("height");
        _mySrc = _myXMLNode->getStringValue("src");

        _myShape = ShapePtr(new Shape());
        ElementPtr element = ElementPtr(new ElementWithTexture());
        size_t dataPerVertex = 3 + 2;
        element->numVertices = 6;
        element->vertexData = boost::shared_array<float>(new float[(element->numVertices) * dataPerVertex]);

        (element->vertexData)[0] = 0.0f;
        (element->vertexData)[1] = 0.0f;
        (element->vertexData)[2] = 0.0f;
        (element->vertexData)[3] = 0.0f;
        (element->vertexData)[4] = 0.0f;

        (element->vertexData)[5] = width;
        (element->vertexData)[6] = 0.0f;
        (element->vertexData)[7] = 0.0f;
        (element->vertexData)[8] = 1.0f;
        (element->vertexData)[9] = 0.0f;

        (element->vertexData)[10] = 0.0f;
        (element->vertexData)[11] = height;
        (element->vertexData)[12] = 0.0f;
        (element->vertexData)[13] = 0.0f;
        (element->vertexData)[14] = 1.0f;

        (element->vertexData)[15] = width;
        (element->vertexData)[16] = 0.0f;
        (element->vertexData)[17] = 0.0f;
        (element->vertexData)[18] = 1.0f;
        (element->vertexData)[19] = 0.0f;

        (element->vertexData)[20] = width;
        (element->vertexData)[21] = height;
        (element->vertexData)[22] = 0.0f;
        (element->vertexData)[23] = 1.0f;
        (element->vertexData)[24] = 1.0f;

        (element->vertexData)[25] = 0.0f;
        (element->vertexData)[26] = height;
        (element->vertexData)[27] = 0.0f;
        (element->vertexData)[28] = 0.0f;
        (element->vertexData)[29] = 1.0f;

        _myShape->elementList.push_back(element);

        //TODO: material inheritance?
        element->material = MaterialPtr(new Material(_myApp->assetProvider, UNLIT_TEXTURED_MATERIAL));
        loadTextureFromPNG(theApp->assetProvider, _mySrc, element->material);

        //material is ready -> create shader now
        element->material->createShader();
    }

    Image::~Image() {
    }

    void Image::renderShape() const {
        _myApp->window->renderShape(_myShape);
    }
}
