#include "Camera.h"
#include "BaseApp.h"

#include "SparkComponentFactory.h"
#include <masl/MobileSDK_Singleton.h>

namespace spark {
    //needed for component factory
    //namespace  {
        ComponentPtr createCamera(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent) {
            return CameraPtr(new Camera(theApp, theXMLNode, theParent));
        };
    //    const bool registered = spark::SparkComponentFactory::get().registerComponent("Rectangle", spark::createRectangle);
    //}

    const char * Camera::SPARK_TYPE = "Camera";

    Camera::Camera(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        ShapeWidget(theApp, theXMLNode, theParent) {
        
        setShape(ShapeFactory::get().createRectangle(true));

        getShape()->setDimensions(500, 500);
        _myDirtyFlag = true;
    }

    Camera::~Camera() {
    }
    
    void 
    Camera::prerender(MatrixStack& theCurrentMatrixStack) {
        ShapeWidget::prerender(theCurrentMatrixStack);    
        if (_myDirtyFlag) {
            _myDirtyFlag = false;
            UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);    
            int myTextureId = MobileSDK_Singleton::get().renderCamera();                                      
            myMaterial->getTexture()->setTextureId(myTextureId);                    
            //myMaterial->transparency_ = true;
        }
    }    
}
