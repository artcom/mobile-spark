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
        _width(0), _height(0), _transparency(false),
        _textureId(0), _mirrorflag(false), _mipmapFlag(false)
    {
        AC_DEBUG << "create Texture " << (void*)this;
    }

    Texture::~Texture() {
        AC_DEBUG << "delete Texture " << (void*)this << " texid: "<<_textureId;
        unbind();
    }

    void
    Texture::setSrc(const std::string & theSrc) {
        AC_DEBUG << "setSrc " << theSrc << " for Texture: " << (void*)this;
        _src = theSrc;
        unbind();
        if (!_src.empty()) {
            masl::AssetProviderSingleton::get().ap()->loadTextureFromFile(theSrc, _textureId, 
                                                                          _width, _height,  
                                                                          _real_width, _real_height,
                                                                          _transparency,
                                                                          _mirrorflag,
                                                                          _mipmapFlag);
            
        } else {
            _width = 0;
            _height = 0;
            _transparency = false;
        }
    }

    void
    Texture::unbind() {
        if (_textureId) {
            AC_DEBUG << "unbind gl texture " << _textureId;
            glDeleteTextures(1, &_textureId);
            _textureId = 0;
        }
    }    

    std::string
    Texture::getAttributesAsString() const {
        return "src=\""+_src+"\" width=\""+masl::as_string(_width)+"\" height=\""+masl::as_string(_height)+"\""
            " transparency=\""+masl::as_string(_transparency)+"\""
            " textureId=\""+masl::as_string(_textureId)+"\"";
    }
    
    uint Texture::nextPowerOfTwo(uint size)
    {
        uint ret = 1;
        while (ret < size)
        {
            // shift
            ret <<= 1;
        }
        
        return ret;
    };
}
