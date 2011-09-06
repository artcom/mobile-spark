#include "Text.h"
#include "BaseApp.h"

#include "SparkComponentFactory.h"
#include <masl/MobileSDK_Singleton.h>

namespace spark {
    //needed for component factory
    //namespace  {
        ComponentPtr createText(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent) {
            return TextPtr(new Text(theApp, theXMLNode, theParent));
        };
    //    const bool registered = spark::SparkComponentFactory::get().registerComponent("Rectangle", spark::createRectangle);
    //}


    Text::Text(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        ShapeWidget(theApp, theXMLNode, theParent) {

        _myText = _myXMLNode->getAttributeAs<std::string>("text", "");

        setShape(ShapeFactory::get().createRectangle(true));

        getShape()->setDimensions(500, 500);
        _myDirtyFlag = true;
    }

    Text::~Text() {
    }
    
    void 
    Text::prerender(MatrixStack& theCurrentMatrixStack) {
        ShapeWidget::prerender(theCurrentMatrixStack);    
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);            
        if (_myDirtyFlag) {
            _myDirtyFlag = false;
            UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);    
            int myTextureId = MobileSDK_Singleton::get().renderText(_myText, myMaterial->getTexture()->getTextureId());                                       
            myMaterial->getTexture()->setTextureId(myTextureId);                    
            myMaterial->transparency_ = true;
        }
    }    
}
