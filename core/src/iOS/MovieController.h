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
#import <CoreVideo/CVOpenGLESTextureCache.h>

#include "masl/Ptr.h"


namespace ios {
    
    class MovieController 
    {
    private:
        //AVAssetReaderTrackOutput *m_trackOutput;
        
        GLuint textureID;
        int width;
        int height;
        
        CVOpenGLESTextureRef _bgraTexture;
        CVOpenGLESTextureCacheRef _videoTextureCache;
        
        typedef masl::Ptr<struct AVStruct> AVStructPtr;
        AVStructPtr _avStruct;
        
    public:
        MovieController();
        virtual ~MovieController();
        
        void playMovie(const std::string &filePath);
        void stop();
        void pause();
        void reset();
        
        void copyNextFrameToTexture();
        
        GLuint getTextureID(){return textureID;};
        
        int getWidth(){return width;};
        int getHeight(){return height;};
        
    };
    
    typedef masl::Ptr<MovieController> MovieControllerPtr;
}

#endif
