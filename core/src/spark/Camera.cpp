#include "Camera.h"

#include <masl/MobileSDK.h>
#include <masl/Exception.h>

#include "BaseApp.h"
#include "Window.h"
#include "SparkComponentFactory.h"

namespace spark {

    const char * const Camera::SPARK_TYPE = "Camera";

    Camera::Camera(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        ShapeWidget(theApp, theXMLNode) {

        setShape(mar::ShapeFactory::get().createRectangle(true));
        _myColorConversionFlag = _myXMLNode->getAttributeAs<bool>("cpu_color_conversion", false);

    }

    Camera::~Camera() {
    }

    void
    Camera::realize() {
        ShapeWidget::realize();
    }

    void
    Camera::onPause() {
        ShapeWidget::onPause();
        if (masl::MobileSDK_Singleton::get().getNative()->isCameraCapturing()) {
            masl::MobileSDK_Singleton::get().getNative()->stopCameraCapture();
        }
    }

    void
    Camera::prerender(MatrixStack& theCurrentMatrixStack) {
        ShapeWidget::prerender(theCurrentMatrixStack);
        if (isRendered()) {
            if (!masl::MobileSDK_Singleton::get().getNative()->isCameraCapturing()) {
                masl::MobileSDK_Singleton::get().getNative()->startCameraCapture(_myColorConversionFlag);
            }
            WindowPtr myWindow = boost::static_pointer_cast<spark::Window>(getRoot());
            masl::CameraInfo myCameraInfo = masl::MobileSDK_Singleton::get().getNative()->getCameraSpec();
            float width = _myXMLNode->getAttributeAs<float>("width", myCameraInfo.width);
            float height = _myXMLNode->getAttributeAs<float>("height", myCameraInfo.height);
            float myShapeWidth = (myWindow->getOrientation() == Orientation::PORTRAIT) ? height : width;
            if (!getShape() || (myShapeWidth != 0 && myShapeWidth != getShape()->getWidth())) {
                setGeometry();
            }
            masl::MobileSDK_Singleton::get().getNative()->updateCameraTexture();
            mar::UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<mar::UnlitTexturedMaterial>(getShape()->elementList_[0]->material);
            if (myCameraInfo.textureID != 0 && myCameraInfo.textureID != myMaterial->getTexture()->getTextureId()) {
                myMaterial->getTexture()->setTextureId(myCameraInfo.textureID);
            }
        } else {
            if (masl::MobileSDK_Singleton::get().getNative()->isCameraCapturing()) {
                masl::MobileSDK_Singleton::get().getNative()->stopCameraCapture();
            }
        }
    }

    void
    Camera::setGeometry() {
        WindowPtr myWindow = boost::static_pointer_cast<spark::Window>(getRoot());
    	masl::CameraInfo myCameraInfo = masl::MobileSDK_Singleton::get().getNative()->getCameraSpec();
        float width = _myXMLNode->getAttributeAs<float>("width", myCameraInfo.width);
		float height = _myXMLNode->getAttributeAs<float>("height", myCameraInfo.height);
		if (myWindow->getOrientation() == Orientation::PORTRAIT) {
			setSize(height, width);
            getShape()->setTexCoords(vector2(width/myCameraInfo.texturewidth,height/myCameraInfo.textureheight) ,
                                     vector2(width/myCameraInfo.texturewidth,0),
                                     vector2(0,height/myCameraInfo.textureheight),
                                     vector2(0,0));
        } else {
			setSize(width, height);
            getShape()->setTexCoords(vector2(0,height/myCameraInfo.textureheight),
            						 vector2(width/myCameraInfo.texturewidth,height/myCameraInfo.textureheight) ,
                                     vector2(0,0),
                                     vector2(width/myCameraInfo.texturewidth,0));
        }
		AC_INFO<< "Camera width x height : " << width << " x " << height << " orientation: "<<myWindow->getOrientation();
    }
}
