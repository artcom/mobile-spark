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

    TextureUnit::TextureUnit() : matrix_(cml::identity_4x4()), _myTextureScaleCorrection(cml::identity_4x4()), _myTexture(TexturePtr(new Texture())) {
    }
    
    TextureUnit::TextureUnit(const TexturePtr theTexture){
        setTexture(theTexture);
    }

    TextureUnit::~TextureUnit() {
    }
    
    void 
    TextureUnit::setTexture(TexturePtr theTexture) {
        matrix_ = cml::identity_4x4();
        _myTextureScaleCorrection = cml::identity_4x4();
        _myTexture = theTexture;
        if (_myTexture->mirrorflag_) {
            matrix scalematrix;        
            cml::matrix_scale(scalematrix, 1.0f, -1.0f, 1.0f);
    
            matrix translatematrix;        
            cml::matrix_translation(translatematrix, 0.0f, 1.0f);
    
            _myTextureScaleCorrection = translatematrix * scalematrix;
        }
    }
    
    matrix & 
    TextureUnit::getMatrix() {
        return matrix_;
     }

    matrix
    TextureUnit::getRenderMatrix() {
        return matrix_ * _myTextureScaleCorrection;
     }

    std::string
    TextureUnit::getAttributesAsString() const {
        return "matrix=\""+masl::as_string(matrix_)+"\" "+_myTexture->getAttributesAsString();
    }

}

