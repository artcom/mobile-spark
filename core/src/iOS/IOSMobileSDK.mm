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

    void IOSMobileSDK::vibrate(long theDurationMillisec) 
    {
        
    }
    
    bool IOSMobileSDK::playMovie(spark::MoviePtr theMovieWidget) 
    {
        std::string filePath;
        
        if (theMovieWidget->getSrc().size() > 0 ) {
            if (!masl::searchFile(theMovieWidget->getSrc(), filePath)) 
                return false;
        }
        
        MovieMap::const_iterator it = _movieMap.find(theMovieWidget);
        if(it == _movieMap.end())
            _movieMap[theMovieWidget] = MovieControllerPtr(new MovieController);
        
        _movieMap[theMovieWidget]->playMovie(filePath);
        
        return true;
    }
    
    void IOSMobileSDK::stopMovie(spark::MoviePtr theMovieWidget) 
    {
        _movieMap[theMovieWidget]->stop();
        
        _movieMap.erase(theMovieWidget);
    }
    
    void IOSMobileSDK::pauseMovie(spark::MoviePtr theMovieWidget) 
    {
        _movieMap[theMovieWidget]->pause();
    }
    
    void IOSMobileSDK::resetMovie(spark::MoviePtr theMovieWidget) 
    {
        _movieMap[theMovieWidget]->reset();
    }
    
    void IOSMobileSDK::updateMovieTexture(spark::MoviePtr theMovieWidget)
    {
        MovieMap::iterator it = _movieMap.find(theMovieWidget);
        
        if(it != _movieMap.end())
            it->second->copyNextFrameToTexture();
    }
    
    const masl::VideoInfo IOSMobileSDK::getMovieInfo(spark::MoviePtr theMovieWidget) const
    {
        masl::VideoInfo movieInfo;
        
        MovieMap::const_iterator it = _movieMap.find(theMovieWidget);
        
        if(it != _movieMap.end())
        {
            movieInfo.textureID = it->second->getTextureID();
            movieInfo.width = it->second->getWidth();
            movieInfo.height = it->second->getHeight();
        }
        
        return movieInfo;
    }
    
    bool IOSMobileSDK::isMoviePlaying(spark::MoviePtr theMovieWidget) const
    {
        bool ret = false;
        
        MovieMap::const_iterator it = _movieMap.find(theMovieWidget);
        
        if(it != _movieMap.end())
        {
            ret = it->second->isPlaying();
        }

        return ret;
    }
    
    void IOSMobileSDK::updateCameraTexture()
    {
        // not needed currently (@see Camera.h)
    }
    
    masl::TextInfo IOSMobileSDK::renderText(const std::string & theMessage, unsigned int theTextureId, int theFontSize, vector4 theColor, 
                                            int theMaxWidth, int theMaxHeight, const std::string & theAlign, const std::string & theFontPath, 
                                            int theLineHeight, int theStartIndex, bool & mirrorFlag) {
        mirrorFlag = false;                                            
        masl::TextInfo textInfo;        
        
        TextRendererPtr textRenderer = TextRendererPtr(new TextRenderer());
        textRenderer->renderText(theMessage.substr(theStartIndex), theTextureId, theFontSize, theColor, (float) theMaxWidth, (float) theMaxHeight, theAlign, theFontPath,theLineHeight, theStartIndex);
        
        textInfo.textureID = textRenderer.get()->getTextureID();
        textInfo.height = textRenderer.get()->getTextureHeight();
        textInfo.width = textRenderer.get()->getTextureWidth();
        textInfo.renderedGlyphIndex = textRenderer.get()->getRenderedGlyphIndex();
        return textInfo;
    }
    
    void IOSMobileSDK::setMovieVolume(spark::MoviePtr theMovieWidget, float newVolume)
    {
        MovieMap::const_iterator it = _movieMap.find(theMovieWidget);
        
        if(it != _movieMap.end())
        {
            it->second->setVolume(newVolume);
        }
    }
    
    bool IOSMobileSDK::loadTextureFromFile(const std::string & filename, unsigned int & textureId, 
                                           unsigned int & width, unsigned int & height, 
                                           unsigned int & real_width, unsigned int & real_height, bool & hasAlpha, bool & theMipmapFlag) 
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
        
        // max gl texture size or imageloader boundary checks nyi
        
        real_width = width = CGImageGetWidth(cgImage);
        real_height = height = CGImageGetHeight(cgImage);
        
        // determine alpha info
        CGImageAlphaInfo alphaInfo = CGImageGetAlphaInfo(cgImage);
        
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
        
        if (alphaInfo == kCGImageAlphaPremultipliedLast) 
        {
            AC_DEBUG << "alpha";
            
            hasAlpha = true;
        } 
        else if (alphaInfo == kCGImageAlphaNoneSkipLast) 
        {
            AC_DEBUG << "no alpha";
            
            // RGBX format, where we want to skip X
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            
            hasAlpha = false;
        } 
        else 
        {
            AC_DEBUG << "unknown color type " << alphaInfo;
        }
        
       
        // ios can only build mipmap tree for pot textures, by now this cannot be guaranteed
        // http://www.khronos.org/webgl/wiki/WebGL_and_OpenGL_Differences#Non-Power_of_Two_Texture_Support
        
        if (theMipmapFlag) 
        {
            
            // get next power of two for width and height
            uint potWidth = mar::Texture::nextPowerOfTwo(real_width);
            uint potHeight = mar::Texture::nextPowerOfTwo(real_height);
            
            width = potWidth;
            height = potHeight;

            //TODO: remove
//            printf("real_width: %d, real_height: %d   --  potWidth: %d, potHeight: %d\n",
//                   real_width, real_height, potWidth, potHeight);
            
            // create empty texture object with pot-dimensions
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, NULL);
            
            // upload with glTexSubImage
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, real_width, real_height,
                            GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) data);
            
            // set filtering to mipmap/linear
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            
            // generate mipmap pyramid
            glGenerateMipmap(GL_TEXTURE_2D);
            
        } 
        else 
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, real_width, real_height, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, (GLvoid*) data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        
        // unbind the texture object again
        glBindTexture(GL_TEXTURE_2D, 0);
        
        AC_DEBUG << "generated texture with id" << textureId;

        // clean up data
        delete [] data;
        
        return true;
    }

    void IOSMobileSDK::freezeMobileOrientation(const std::string & theOrientation) 
    {
        m_orientationString = theOrientation;
    }
    
    float IOSMobileSDK::getDeviceBatteryLevel()
    {
        [[UIDevice currentDevice] setBatteryMonitoringEnabled:YES];
        
        //[[UIDevice currentDevice] batteryState];
        
        return [[UIDevice currentDevice] batteryLevel];
    }
    
    masl::VideoInfo IOSMobileSDK::getCameraSpec() {
        Camera * myCamera = [Camera instance];
        masl::VideoInfo cameraInfo;
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
