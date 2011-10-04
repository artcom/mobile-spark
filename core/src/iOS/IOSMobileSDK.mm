#include "IOSMobileSDK.h"

#include "TextRenderer.h"
#include <masl/Logger.h>


namespace ios 
{
    IOSMobileSDK::IOSMobileSDK() {}

    IOSMobileSDK::~IOSMobileSDK() {}

    void IOSMobileSDK::vibrate(long theDurationMillisec) {
    }

    masl::TextInfo IOSMobileSDK::renderText(const std::string & theMessage, unsigned int theTextureId, int theFontSize, vector4 theColor, int theMaxWidth, int theMaxHeight, const std::string & theAlign, const std::string & theFontPath) {
        masl::TextInfo textInfo;        
        
        TextRendererPtr textRenderer = TextRendererPtr(new TextRenderer());
        textRenderer.get()->renderText(theMessage, theTextureId, theFontSize, theColor, (float) theMaxWidth, (float) theMaxHeight, theAlign, theFontPath);
        
        textInfo.textureID = textRenderer.get()->getTextureID();
        textInfo.height = textRenderer.get()->getTextureHeight();
        textInfo.width = textRenderer.get()->getTextureWidth();
                
        return textInfo;
    }

    void IOSMobileSDK::updateCameraTexture() {
    
    }
    
    void IOSMobileSDK::freezeMobileOrientation(const std::string & theOrientation) {
        
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
