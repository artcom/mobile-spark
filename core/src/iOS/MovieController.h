// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_ios_MovieController_
#define _included_mobile_ios_MovieController_

#import <OpenGLES/ES2/gl.h>
//#import <CoreVideo/CVOpenGLESTextureCache.h>
#import <CoreVideo/CoreVideo.h>

#include "masl/Ptr.h"


namespace ios {
    
    class MovieController 
    {
    private:
        
        GLuint _textureID;
        int _width;
        int _height;
        
        CVOpenGLESTextureRef _bgraTexture;
        CVOpenGLESTextureCacheRef _videoTextureCache;
        
        typedef masl::Ptr<struct AVStruct> AVStructPtr;
        AVStructPtr _avStruct;
        
        void pixelBufferToGLTexture(const CVPixelBufferRef pixelBuf,
                                    GLuint &textureName);
        
        void pixelBufferToGLTexture_oldschool(const CVPixelBufferRef pixelBuf,
                                    GLuint &textureName);
        
    public:
        MovieController();
        virtual ~MovieController();
        
        void playMovie(const std::string &filePath);
        void stop();
        void pause();
        void reset();
        
        void copyNextFrameToTexture();
        void copyNextFrameToTexture2();
        
        GLuint getTextureID(){return _textureID;};
        
        int getWidth(){return _width;};
        int getHeight(){return _height;};
        
    };
    
    typedef masl::Ptr<MovieController> MovieControllerPtr;
}

#endif
