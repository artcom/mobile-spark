#include "NinePatch.h"

#include <mar/png_functions.h>
#include "BaseApp.h"
#include "SparkComponentFactory.h"

namespace spark {
    //needed for component factory
    //namespace  {
        ComponentPtr createNinePatch(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent) {
            return NinePatchPtr(new NinePatch(theApp, theXMLNode, theParent));
        };
    //    const bool registered = spark::SparkComponentFactory::get().registerComponent("NinePatch", spark::createNinePatch);
    //}


    NinePatch::NinePatch(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        ShapeWidget(theApp, theXMLNode, theParent) {

        src_ = _myXMLNode->getStringValue("src");
        setShape(ShapeFactory::get().createRectangle(true, src_));
        UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);    
        //loadTextureFromPNG(_mySrc, myMaterial);
        float width = _myXMLNode->getFloatValue("width", myMaterial->getTexture()->width);
        float height = _myXMLNode->getFloatValue("height", myMaterial->getTexture()->height);
        getShape()->setDimensions(width, height);

        edgeLeft_ = _myXMLNode->getFloatValue("edgeLeft");
        edgeTop_ = _myXMLNode->getFloatValue("edgeTop");
        edgeRight_ = _myXMLNode->getFloatValue("edgeRight");
        edgeBottom_ = _myXMLNode->getFloatValue("edgeBottom");
    }

    NinePatch::~NinePatch() {
    }        
}
