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
        edgeLeft_ = _myXMLNode->getFloatValue("edgeLeft", 0);
        edgeTop_ = _myXMLNode->getFloatValue("edgeTop", 0);
        edgeRight_ = _myXMLNode->getFloatValue("edgeRight", 0);
        edgeBottom_ = _myXMLNode->getFloatValue("edgeBottom", 0);
        AC_PRINT << " create ninepatch with edges " << edgeLeft_ << " " << edgeTop_ << " " << edgeRight_ << "  " << edgeBottom_;
        
        float width = _myXMLNode->getFloatValue("width", -1);
        float height = _myXMLNode->getFloatValue("height", -1);

        setShape(ShapeFactory::get().createNinePatch(src_, edgeLeft_, edgeTop_, edgeRight_, edgeBottom_, 
                    width >= 0 ? width : 0, height >= 0 ? height : 0));
        UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);    
        if (width == -1 || height == -1) {
            width = width == -1 ? myMaterial->getTexture()->width : width;
            height = height == -1 ? myMaterial->getTexture()->height : height;
            getShape()->setDimensions(width, height);
        }
    }

    NinePatch::~NinePatch() {
    }        
}
