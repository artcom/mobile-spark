#include "Image.h"

#include <mar/png_functions.h>
#include "BaseApp.h"
#include "SparkComponentFactory.h"

namespace spark {
    //needed for component factory
    namespace  {
        const bool registered = spark::SparkComponentFactory::get().registerComponent("Image", spark::create<Image>);
    }


    Image::Image(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        ShapeWidget(theApp, theXMLNode, theParent) {

        _mySrc = _myXMLNode->getAttributeAs<std::string>("src","");
        build();
    }

    void 
    Image::onResume() {        
        ShapeWidget::onResume();        
        build();
    }
    
    void
    Image::build() {
        float width = _myXMLNode->getAttributeAs<float>("width", -1);
        float height = _myXMLNode->getAttributeAs<float>("height", -1);
        setShape(ShapeFactory::get().createRectangle(true, width >= 0 ? width : 0, height >= 0 ? height : 0, _mySrc));
        UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);    
        if (width == -1 || height == -1) {
            width = width == -1 ? myMaterial->getTexture()->width_ : width;
            height = height == -1 ? myMaterial->getTexture()->height_ : height;
            //dimensions have to be set after image size is known
            getShape()->setDimensions(width, height);
        }
    }
    
    Image::~Image() {
    }        
}
