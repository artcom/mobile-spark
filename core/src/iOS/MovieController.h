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
#import <CoreVideo/CoreVideo.h>

#include "masl/Ptr.h"

/*
* This class controls playback of Movie-widgets and manages their assets.
* There is 1:1 correlation between spark::Movie object and MovieControllers,
* each controller beeing responsible for exactly one widget.
* Objective-C classes are wrapped in a forward declared C-struct (AVStruct) for compatibility reasons.
*
* Due to planned experiments this class is still a little messy :(
*/

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
        
        std::string _srcPath;
        bool _playing;
        
        void pixelBufferToGLTexture(const CVPixelBufferRef pixelBuf,
                                    GLuint &textureName);
        
        void pixelBufferToGLTexture_compatibility(const CVPixelBufferRef pixelBuf,
                                    GLuint &textureName);
        
    public:
        MovieController();
        virtual ~MovieController();
        
        void playMovie(const std::string &filePath);
        void stop();
        void pause();
        void reset();
        
        float getVolume() const;
        void setVolume(float newVolume);
        
        bool isPlaying() const;
        
        void copyNextFrameToTexture();
        
        GLuint getTextureID(){return _textureID;};
        
        int getWidth(){return _width;};
        int getHeight(){return _height;};
        
    };
    
    typedef masl::Ptr<MovieController> MovieControllerPtr;
}

#endif
