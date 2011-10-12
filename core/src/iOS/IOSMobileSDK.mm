#include "IOSMobileSDK.h"

#include "TextRenderer.h"
#include <masl/Logger.h>

#import "Camera.h"

namespace ios 
{
    IOSMobileSDK::IOSMobileSDK(){
            }

    IOSMobileSDK::~IOSMobileSDK() {}

    void IOSMobileSDK::vibrate(long theDurationMillisec) {
    }

    masl::TextInfo IOSMobileSDK::renderText(const std::string & theMessage, unsigned int theTextureId, int theFontSize, vector4 theColor, 
                                            int theMaxWidth, int theMaxHeight, const std::string & theAlign, const std::string & theFontPath, 
                                            int theLineHeight, int theStartIndex) {
        masl::TextInfo textInfo;        
        
        TextRendererPtr textRenderer = TextRendererPtr(new TextRenderer());
        textRenderer.get()->renderText(theMessage.substr(theStartIndex), theTextureId, theFontSize, theColor, (float) theMaxWidth, (float) theMaxHeight, theAlign, theFontPath,theLineHeight);
        
        textInfo.textureID = textRenderer.get()->getTextureID();
        textInfo.height = textRenderer.get()->getTextureHeight();
        textInfo.width = textRenderer.get()->getTextureWidth();
        textInfo.renderedGlyphIndex = textRenderer.get()->getRenderedGlyphIndex();
        return textInfo;
    }

    void IOSMobileSDK::freezeMobileOrientation(const std::string & theOrientation) {
        
    }
    masl::CameraInfo IOSMobileSDK::getCameraSpec() {
        Camera * myCamera = [Camera instance];
        masl::CameraInfo cameraInfo;
        cameraInfo.width = [myCamera getWidth];
        cameraInfo.height = [myCamera getHeight];
        cameraInfo.texturewidth = [myCamera getWidth];
        cameraInfo.textureheight = [myCamera getHeight];
        cameraInfo.textureID = [myCamera getTextureID];

        return cameraInfo;
    }
    void IOSMobileSDK::startCameraCapture(bool /*theColorConversionFlag*/) {
        [[Camera instance] startCameraCapture];
    }

    void IOSMobileSDK::updateCameraTexture() {
        [[Camera instance] updateCameraTexture];
    }
    
    void IOSMobileSDK::stopCameraCapture() {
        [[Camera instance] stopCameraCapture];
    }
    
    bool IOSMobileSDK::isCameraCapturing() {
        return [[Camera instance] isCameraCapturing];
    }
}
