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
        ShapeWidget(theApp, theXMLNode, theParent) {
        _mySrc = _myXMLNode->getStringValue("src");

        _myShape = ShapePtr(new Shape());
        ElementPtr element = ElementPtr(new ElementWithTexture());
        UnlitTexturedMaterialPtr myMaterial = UnlitTexturedMaterialPtr(new UnlitTexturedMaterial(_myApp->assetProvider));
        loadTextureFromPNG(theApp->assetProvider, _mySrc, myMaterial);
        element->material = myMaterial;
        element->material->createShader();

        float width = _myXMLNode->getFloatValue("width", myMaterial->width);
        float height = _myXMLNode->getFloatValue("height", myMaterial->height);
        setVertexData(element, width, height);
        _myShape->elementList.push_back(element);
    }

    Image::~Image() {
    }

    void Image::setVertexData(ElementPtr theElement, const float theWidth, const float theHeight) {
        size_t dataPerVertex = 3 + 2;
        theElement->numVertices = 6;
        theElement->vertexData = boost::shared_array<float>(new float[(theElement->numVertices) * dataPerVertex]);
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
