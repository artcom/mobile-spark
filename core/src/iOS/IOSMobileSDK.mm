// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

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
        textRenderer.get()->renderText(theMessage.substr(theStartIndex), theTextureId, theFontSize, theColor, (float) theMaxWidth, (float) theMaxHeight, theAlign, theFontPath,theLineHeight, theStartIndex);
        
        textInfo.textureID = textRenderer.get()->getTextureID();
        textInfo.height = textRenderer.get()->getTextureHeight();
        textInfo.width = textRenderer.get()->getTextureWidth();
        textInfo.renderedGlyphIndex = textRenderer.get()->getRenderedGlyphIndex();
        return textInfo;
    }
    
    bool IOSMobileSDK::loadTextureFromFile(const std::string & filename, unsigned int & textureId, 
                                           unsigned int & width, unsigned int & height, bool & hasAlpha) {
       return true;
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
    
    void IOSMobileSDK::exit() {
        AC_PRINT << "iOS apps, can't be quit";
    }
}
