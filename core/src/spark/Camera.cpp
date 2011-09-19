#include "Camera.h"
#include "BaseApp.h"

#include "SparkComponentFactory.h"
#include <masl/MobileSDK.h>

namespace spark {

    const char * const Camera::SPARK_TYPE = "Camera";

    Camera::Camera(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        ShapeWidget(theApp, theXMLNode, theParent),_mySetupFlag(false), _myPortraitMode(true) {

        setShape(ShapeFactory::get().createRectangle(true));
        _myColorConversionFlag = _myXMLNode->getAttributeAs<bool>("cpu_color_conversion", false);
        getShape()->setDimensions(500, 500);

        CameraPtr ptr = boost::static_pointer_cast<Camera>(shared_from_this());
        EventCallbackPtr mySizeChangedCB = EventCallbackPtr(new CameraCB(ptr, &Camera::onSizeChanged));
        WindowPtr myWindow = boost::static_pointer_cast<spark::Window>(getRoot());
        myWindow->addEventListener(WindowEvent::ON_RESIZE, mySizeChangedCB);
    }

    Camera::~Camera() {
    }

    void
    Camera::onPause() {
        ShapeWidget::onPause();
		MobileSDK_Singleton::get().getNative()->stopCameraCapture();
    }

    void
    Camera::onSizeChanged(EventPtr theEvent) {
        WindowEventPtr myEvent = boost::static_pointer_cast<WindowEvent>(theEvent);
        float myRatio = myEvent->size_[0] /myEvent->size_[1];
        if (myRatio < 1.0) {
            _myPortraitMode = true;
            AC_PRINT << "Camera::onSizeChanged senkrecht";
        } else {
            _myPortraitMode = false;
            AC_PRINT << "Camera::onSizeChanged waagerecht";
        }
        _mySetupFlag = false;
    }

    void
    Camera::prerender(MatrixStack& theCurrentMatrixStack) {
        ShapeWidget::prerender(theCurrentMatrixStack);
        //AC_PRINT << "camera us capturing : " << MobileSDK_Singleton::get().isCameraCapturing();
        if (isRendered()) {
            if (!MobileSDK_Singleton::get().getNative()->isCameraCapturing()) {
    			MobileSDK_Singleton::get().getNative()->startCameraCapture(_myColorConversionFlag);
            }
            if (MobileSDK_Singleton::get().getNative()->isCameraCapturing() && !_mySetupFlag) {
                _mySetupFlag = true;
                masl::CameraInfo myCameraInfo = MobileSDK_Singleton::get().getNative()->getCameraSpec();
                if (myCameraInfo.textureID != 0) {
            		UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList[0]->material);
        			myMaterial->getTexture()->setTextureId(myCameraInfo.textureID);
                    setGeometry();
                }
            }


		    MobileSDK_Singleton::get().getNative()->updateCameraTexture();
        } else {
            if (MobileSDK_Singleton::get().getNative()->isCameraCapturing()) {
    			MobileSDK_Singleton::get().getNative()->stopCameraCapture();
            }
        }
    }

    void
    Camera::setGeometry() {
    	masl::CameraInfo myCameraInfo = MobileSDK_Singleton::get().getNative()->getCameraSpec();
        float width = _myXMLNode->getAttributeAs<float>("width", myCameraInfo.width);
		float height = _myXMLNode->getAttributeAs<float>("height", myCameraInfo.height);
		if (_myPortraitMode) {
			getShape()->setDimensions(height, width);
            getShape()->setTexCoords(vector2(width/myCameraInfo.texturewidth,height/myCameraInfo.textureheight) ,
                                     vector2(width/myCameraInfo.texturewidth,0),
                                     vector2(0,height/myCameraInfo.textureheight),
                                     vector2(0,0));
        } else {
			getShape()->setDimensions(width, height);
            getShape()->setTexCoords(vector2(0,height/myCameraInfo.textureheight),
            						 vector2(width/myCameraInfo.texturewidth,height/myCameraInfo.textureheight) ,
                                     vector2(0,0),
                                     vector2(width/myCameraInfo.texturewidth,0));
        }
		AC_PRINT<< " ####### Camera width x height : " << width << " x " << height;
    }
}
