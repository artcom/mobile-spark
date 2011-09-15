#include "IOSMobileSDK.h"

#include "TextRenderer.h"
#include <masl/Logger.h>


namespace ios 
{
    IOSMobileSDK::IOSMobileSDK() {}

    IOSMobileSDK::~IOSMobileSDK() {}

    masl::TextInfo IOSMobileSDK::renderText(const std::string & theMessage, int theTextureId, int theFontSize, vector4 theColor, int theMaxWidth, int theMaxHeight, const std::string & theAlign, const std::string & theFontPath) {
        AC_PRINT << "xxx: " << theMessage;
        masl::TextInfo textInfo;
        
        TextRendererPtr test = TextRendererPtr(new TextRenderer());
        AC_PRINT << "tex id before: " << theTextureId;
        test.get()->renderText(theMessage, theTextureId, theFontSize, theColor, 100, 100);
        textInfo.textureID = test.get()->getTextureID();
        AC_PRINT << "tex id after: " << textInfo.textureID;
        textInfo.height = 100;
        textInfo.width = 100;
        
        return textInfo;
    }

    void IOSMobileSDK::updateCameraTexture() {
    
    }
    
    void IOSMobileSDK::freezeMobileOrientation(std::string theOrientation) {
        
    }
    masl::CameraInfo IOSMobileSDK::getCameraSpec() {
        masl::CameraInfo cameraInfo;
        
        return cameraInfo;
    }
    void IOSMobileSDK::startCameraCapture(bool theColorConversionFlag) {
    
    }
    void IOSMobileSDK::stopCameraCapture() {
    
    }
    bool IOSMobileSDK::isCameraCapturing() {
        return false;
    }
}