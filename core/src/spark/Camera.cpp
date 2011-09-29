#include "Camera.h"

#include <masl/MobileSDK.h>
#include <masl/Exception.h>

#include "BaseApp.h"
#include "Window.h"
#include "SparkComponentFactory.h"

namespace spark {

    const char * const Camera::SPARK_TYPE = "Camera";

    Camera::Camera(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        ShapeWidget(theApp, theXMLNode), _myPortraitMode(true) {

        setShape(mar::ShapeFactory::get().createRectangle(true));
        _myColorConversionFlag = _myXMLNode->getAttributeAs<bool>("cpu_color_conversion", false);
        getShape()->setDimensions(500, 500);

    }

    Camera::~Camera() {
    }

    void
    Camera::realize() {
        ShapeWidget::realize();
        CameraPtr ptr = boost::static_pointer_cast<Camera>(shared_from_this());
        EventCallbackPtr mySizeChangedCB = EventCallbackPtr(new CameraCB(ptr, &Camera::onSizeChanged));
        WindowPtr myWindow = boost::static_pointer_cast<spark::Window>(getRoot());
        myWindow->addEventListener(WindowEvent::ON_RESIZE, mySizeChangedCB);

    }

    void
    Camera::onPause() {
        ShapeWidget::onPause();
        masl::MobileSDK_Singleton::get().getNative()->stopCameraCapture();
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
    }

    void
    Camera::prerender(MatrixStack& theCurrentMatrixStack) {
        ShapeWidget::prerender(theCurrentMatrixStack);
        //AC_PRINT << "camera us capturing : " << MobileSDK_Singleton::get().isCameraCapturing();
        if (isRendered()) {
            if (!masl::MobileSDK_Singleton::get().getNative()->isCameraCapturing()) {
                masl::MobileSDK_Singleton::get().getNative()->startCameraCapture(_myColorConversionFlag);
            }
            masl::CameraInfo myCameraInfo = masl::MobileSDK_Singleton::get().getNative()->getCameraSpec();
            mar::UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<mar::UnlitTexturedMaterial>(getShape()->elementList[0]->material);
            if (myCameraInfo.textureID != 0 && myCameraInfo.textureID != myMaterial->getTexture()->getTextureId()) {
    			myMaterial->getTexture()->setTextureId(myCameraInfo.textureID);
                setGeometry();
            }
            masl::MobileSDK_Singleton::get().getNative()->updateCameraTexture();
        } else {
            if (masl::MobileSDK_Singleton::get().getNative()->isCameraCapturing()) {
                masl::MobileSDK_Singleton::get().getNative()->stopCameraCapture();
            }
        }
    }

    void
    Camera::setGeometry() {
    	masl::CameraInfo myCameraInfo = masl::MobileSDK_Singleton::get().getNative()->getCameraSpec();
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
