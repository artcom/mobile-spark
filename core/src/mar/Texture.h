// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_mar_Texture_h_included_
#define _ac_mobile_mar_Texture_h_included_

#include <boost/enable_shared_from_this.hpp>
#include <masl/Ptr.h>
#include "openGL_functions.h"

namespace mar {
    class Texture : public boost::enable_shared_from_this<Texture> {
        public:
            Texture();
            ~Texture();
            void unbind();
            std::string getSrc() const {return src_;};
            void setSrc(const std::string & theSrc = "");
            std::string getAttributesAsString() const;

            GLuint width_; 
            GLuint height_;
            GLuint real_width_;  // in some cases images must be downscaled to be loaded from android/ios 
            GLuint real_height_; // imageloader or fit max gl texture size, this is the real size
            bool transparency_;
            GLuint textureId_;     
            bool mirrorflag_;  
            bool mipmapFlag_; 
        private:
            std::string src_;
        
    };
    typedef masl::Ptr<Texture> TexturePtr;
        
};

#endif
