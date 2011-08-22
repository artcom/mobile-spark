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

        std::vector<float> _myColor;
        _myColor.push_back(1.0);
        _myColor.push_back(0.0);
        _myColor.push_back(1.0);
        _myColor.push_back(1.0);
        element->material->diffuse = _myColor;

        element->material->textureFile = _mySrc; //needed?
        int imageWidth, imageHeight;
        bool rgb;
        //TODO: zip should be android
        zip* myPackage = boost::static_pointer_cast<android::AndroidAssetProvider>(theApp->assetProvider)->getAPK();
        element->material->textureId = loadTextureFromPNG(myPackage, _mySrc, imageWidth, imageHeight, rgb);
        element->material->rgb = rgb;
        element->material->createShader();
 
        //_myShape->materialMap["xxx"] = element->material; //XXX: needed?
    }

    Image::~Image() {
    }

    void Image::renderShape() const {
        _myApp->window->renderShape(_myShape);
    }
}
