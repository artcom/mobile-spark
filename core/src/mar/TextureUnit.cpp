// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "TextureUnit.h"

#include <masl/Logger.h>
#include <masl/numeric_functions.h>

namespace mar {

    TextureUnit::TextureUnit() :
        _matrix(cml::identity_4x4()),
        _textureScaleCorrection(cml::identity_4x4()),
        _texture(TexturePtr(new Texture()))
    {}
    
    TextureUnit::TextureUnit(const TexturePtr theTexture){
        setTexture(theTexture);
    }

    TextureUnit::~TextureUnit() {
    }
    //TODO: cleanup mirroring
    void 
    TextureUnit::setTexture(TexturePtr theTexture) {
        _matrix = cml::identity_4x4();
        _textureScaleCorrection = cml::identity_4x4();
        _texture = theTexture;
        if (_texture->_mirrorFlag) {
            matrix scalematrix;        
            cml::matrix_scale(scalematrix, 1.0f, -1.0f, 1.0f);
    
            matrix translatematrix;        
            cml::matrix_translation(translatematrix, 0.0f, 1.0f);
    
            _textureScaleCorrection = translatematrix * scalematrix;
        }
    }
    
    matrix
    TextureUnit::getRenderMatrix() const {
        return _matrix * _textureScaleCorrection;
     }

    std::string
    TextureUnit::getAttributesAsString() const {
        return "matrix=\""+masl::as_string(_matrix)+"\" "+_texture->getAttributesAsString();
    }

}

