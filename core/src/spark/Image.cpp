#include "Image.h"

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
        _myShape = ShapeFactory::get().createRectangle(true);
        UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(_myShape->elementList[0]->material);    
        loadTextureFromPNG(_mySrc, myMaterial);
        float width = _myXMLNode->getFloatValue("width", myMaterial->width);
        float height = _myXMLNode->getFloatValue("height", myMaterial->height);
        _myShape->setDimensions(width, height);
    }

    Image::~Image() {
    }
}
