#include "IOSMobileSDK.h"

#include "TextRenderer.h"
#include <masl/Logger.h>


namespace ios 
{
    IOSMobileSDK::IOSMobileSDK() {}

    IOSMobileSDK::~IOSMobileSDK() {}

    masl::TextInfo IOSMobileSDK::renderText(const std::string & theMessage, int theTextureId, int theFontSize, vector4 theColor, int theMaxWidth, int theMaxHeight, const std::string & theAlign, const std::string & theFontPath) {
        masl::TextInfo textInfo;        
        
        TextRendererPtr test = TextRendererPtr(new TextRenderer());
        test.get()->renderText(theMessage, theTextureId, theFontSize, theColor, (float) theMaxWidth, (float) theMaxHeight, theAlign, theFontPath);
        
        textInfo.textureID = test.get()->getTextureID();
        textInfo.height = test.get()->getTextureHeight();
        textInfo.width = test.get()->getTextureWidth();
                
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