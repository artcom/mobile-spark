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
        ShapeWidget(theApp, theXMLNode, theParent), _myCameraCaptureStartedFlag(false), _myCameraReadyFlag(false) {
        
        setShape(ShapeFactory::get().createRectangle(true));

        getShape()->setDimensions(1, 1);
    }

    Camera::~Camera() {
    }
    
    void 
    Camera::prerender(MatrixStack& theCurrentMatrixStack) {
        ShapeWidget::prerender(theCurrentMatrixStack);    
        if (isVisible()) {
    		if  (!_myCameraCaptureStartedFlag) {
    			_myCameraCaptureStartedFlag = true;
    			MobileSDK_Singleton::get().startCameraCapture();
    		}
    		if (!_myCameraReadyFlag) {
                masl::CameraInfo myCameraInfo = MobileSDK_Singleton::get().getCameraSpec();
                if (myCameraInfo.textureID != 0) {
        			_myCameraReadyFlag = 1;
        			float width = _myXMLNode->getAttributeAs<float>("width", myCameraInfo.width);
        			float height = _myXMLNode->getAttributeAs<float>("height", myCameraInfo.height);
            		UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);    
        			myMaterial->getTexture()->setTextureId(myCameraInfo.textureID);
        			getShape()->setDimensions(width, height);
                    getShape()->setTexCoords(vector2(0,0), vector2(width/1024,0), vector2(0,height/512), vector2(width/1024,height/512));
        			AC_PRINT<< " ####### Camera width x height : " << width << " x " << height; 
                }
    		}
    		if (_myCameraReadyFlag) {
    		    MobileSDK_Singleton::get().updateCameraTexture();
    		}                    
            //myMaterial->transparency_ = true;
        } else {
		if  (_myCameraCaptureStartedFlag) {
			_myCameraCaptureStartedFlag = false;
			MobileSDK_Singleton::get().stopCameraCapture();
		}
	}
    }    
}
