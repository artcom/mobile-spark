// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "Texture.h"
#include <masl/Logger.h>
#include <masl/numeric_functions.h>
#include <masl/AssetProvider.h>

namespace mar {

    Texture::Texture() :
        width_(0), height_(0), transparency_(false),
        textureId_(0), mirrorflag_(false)
    {
        AC_DEBUG << "create Texture " << (void*)this;
    }

    Texture::~Texture() {
        AC_DEBUG << "delete Texture " << (void*)this << " texid: "<<textureId_;
        unbind();
    }

    void
    Texture::setSrc(const std::string & theSrc) {
        AC_DEBUG << "setSrc " << theSrc << " for Texture: " << (void*)this;
        src_ = theSrc;
        unbind();
        if (!src_.empty()) {
            masl::AssetProviderSingleton::get().ap()->loadTextureFromFile(theSrc, textureId_, 
                                                                          width_, height_,  
                                                                          transparency_,
                                                                          mirrorflag_);
            
        } else {
            width_ = 0;
            height_ = 0;
            transparency_ = false;
        }
    }

    void
    Texture::unbind() {
        if (textureId_) {
            AC_DEBUG << "unbind gl texture " << textureId_;
            glDeleteTextures(1, &textureId_);
            textureId_ = 0;
        }
    }    

    std::string
    Texture::getAttributesAsString() const {
        return "src=\""+src_+"\" width=\""+masl::as_string(width_)+"\" height=\""+masl::as_string(height_)+"\""
            " transparency=\""+masl::as_string(transparency_)+"\""
            " textureId=\""+masl::as_string(textureId_)+"\"";
    }
}
