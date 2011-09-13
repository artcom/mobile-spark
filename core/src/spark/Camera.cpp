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
        ShapeWidget(theApp, theXMLNode, theParent),_mySetupFlag(false) {
        
        setShape(ShapeFactory::get().createRectangle(true));
        _myColorConversionFlag = _myXMLNode->getAttributeAs<bool>("cpu_color_conversion", false);
        getShape()->setDimensions(500, 500);
    }

    Camera::~Camera() {
    }
    
    void 
    Camera::onPause() {
        ShapeWidget::onPause();        
		MobileSDK_Singleton::get().stopCameraCapture();        
    }
    
    void 
    Camera::prerender(MatrixStack& theCurrentMatrixStack) {
        ShapeWidget::prerender(theCurrentMatrixStack);    
        //AC_PRINT << "camera us capturing : " << MobileSDK_Singleton::get().isCameraCapturing();
        if (isRendered()) {
            if (!MobileSDK_Singleton::get().isCameraCapturing()) {
    			MobileSDK_Singleton::get().startCameraCapture(_myColorConversionFlag);
			}
			if (MobileSDK_Singleton::get().isCameraCapturing() && !_mySetupFlag) {
			    _mySetupFlag = true;
                masl::CameraInfo myCameraInfo = MobileSDK_Singleton::get().getCameraSpec();
                if (myCameraInfo.textureID != 0) {
        			float width = _myXMLNode->getAttributeAs<float>("width", myCameraInfo.width);
        			float height = _myXMLNode->getAttributeAs<float>("height", myCameraInfo.height);
            		UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);    
        			myMaterial->getTexture()->setTextureId(myCameraInfo.textureID);
        			getShape()->setDimensions(width, height);
                    getShape()->setTexCoords(vector2(0,height/myCameraInfo.textureheight),
                    						 vector2(width/myCameraInfo.texturewidth,height/myCameraInfo.textureheight) , 
                                             vector2(0,0), 
                                             vector2(width/myCameraInfo.texturewidth,0));
        			AC_PRINT<< " ####### Camera width x height : " << width << " x " << height; 
                }
            }
		    MobileSDK_Singleton::get().updateCameraTexture();
        } else {
            if (MobileSDK_Singleton::get().isCameraCapturing()) {
    			MobileSDK_Singleton::get().stopCameraCapture();
            }
        }
    }    
}
