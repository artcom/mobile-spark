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
    IOSMobileSDK::IOSMobileSDK()
    {
    
    }

    IOSMobileSDK::~IOSMobileSDK() {}

    void IOSMobileSDK::vibrate(long theDurationMillisec) {
    }
    bool IOSMobileSDK::playMovie(const std::string & theURL) { return true;}
    void IOSMobileSDK::stopMovie() {}
    void IOSMobileSDK::pauseMovie() {}
    void IOSMobileSDK::resetMovie() {}

    masl::TextInfo IOSMobileSDK::renderText(const std::string & theMessage, unsigned int theTextureId, int theFontSize, vector4 theColor, 
                                            int theMaxWidth, int theMaxHeight, const std::string & theAlign, const std::string & theFontPath, 
                                            int theLineHeight, int theStartIndex, bool & mirrorFlag) {
        mirrorFlag = false;                                            
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
                                           unsigned int & width, unsigned int & height, 
                                           unsigned int & real_width, unsigned int & real_height, bool & hasAlpha, bool theMipmapFlag) 
    {
        std::string filePath;
        
        if (filename.size() > 0 ) {
            if (!masl::searchFile(filename, filePath)) 
                return false;
        }
        
        NSString *filepathString = [NSString stringWithUTF8String:filePath.c_str()];
        UIImage* imageClass = [[UIImage alloc] initWithContentsOfFile:filepathString];
        
        CGImageRef cgImage = imageClass.CGImage;
        if (!cgImage)
        {
            [imageClass release];
            return false;
        }
        
        width = CGImageGetWidth(cgImage);
        height = CGImageGetHeight(cgImage);
        
        // determine alpha info
        CGImageAlphaInfo alphaInfo = CGImageGetAlphaInfo(cgImage);
        
        //TODO: remove
        //printf("IMAGE (%d,%d) %d\n",width,height,(unsigned int)alphaInfo);
        
        // we always have 4 components (no alpha with kCGImageAlphaNoneSkipLast)
        unsigned int rowByteSize = width * 4;
        
        char *data = new char[height * rowByteSize];
        
        // see http://developer.apple.com/library/mac/#qa/qa1037/_index.html
        // for a list of supported combinations
        CGContextRef context = CGBitmapContextCreate(data,
                                                     width,
                                                     height,
                                                     8, // bits/component
                                                     rowByteSize,
                                                     CGImageGetColorSpace(cgImage),
                                                     alphaInfo);
        
        CGContextSetBlendMode(context, kCGBlendModeCopy);
        
        // convert to OpenGL coordinate system
        bool flipVertical = true;
        
        if(flipVertical)
        {
            CGContextTranslateCTM(context, 0.0, height);
            CGContextScaleCTM(context, 1.0, -1.0);
        }
        CGContextDrawImage(context, CGRectMake(0.0, 0.0, width, height), cgImage);
        CGContextRelease(context);
        
        // free UIImage
        [imageClass release];
        
        // create our OpenGL texture object
        glGenTextures(1, &textureId);
        
        if(!(textureId && data))
        {
            AC_DEBUG << "texture generation failed";
            delete [] data;
            return false;
        }
        
        // bind the texture object
        glBindTexture(GL_TEXTURE_2D, textureId);
        
        AC_DEBUG << "w x h : " << width << ", " << height;
        
        if (alphaInfo == kCGImageAlphaPremultipliedLast) {
            AC_DEBUG << "alpha";
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, (GLvoid*) data);
            hasAlpha = true;
        } else if (alphaInfo == kCGImageAlphaNoneSkipLast) {
            AC_DEBUG << "no alpha";
            // RGBX format, where we want to skip X
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, (GLvoid*) data);
            hasAlpha = false;
        } else {
            AC_DEBUG << "unknown color type " << alphaInfo;
        }
        
        // http://www.khronos.org/webgl/wiki/WebGL_and_OpenGL_Differences#Non-Power_of_Two_Texture_Support
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        // unbind the texture object again
        glBindTexture(GL_TEXTURE_2D, 0);
        
        AC_DEBUG << "generated texture with id" << textureId;
        // clean up data
        delete [] data;
        
        real_width = width;  // max gl texture size or imageloader boundary checks nyi
        real_height = height;
        return true;
    }

    void IOSMobileSDK::freezeMobileOrientation(const std::string & theOrientation) 
    {
        m_orientationString = theOrientation;
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
    
    std::string IOSMobileSDK::getOrientation()
    {
        return m_orientationString;
    }
}
