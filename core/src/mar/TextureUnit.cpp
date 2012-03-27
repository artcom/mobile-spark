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
        _texture(TexturePtr(new Texture())),
        _matrix(cml::identity_4x4())
    {}

    TextureUnit::TextureUnit(const TexturePtr theTexture) :
        _matrix(cml::identity_4x4())
    {
        setTexture(theTexture);
    }

    TextureUnit::~TextureUnit() {
    }

    matrix
    TextureUnit::getRenderMatrix() const {
        return _matrix * _texture->getRenderMatrix();
    }

    std::string
    TextureUnit::getAttributesAsString() const {
        return "matrix=\"" + masl::as_string(_matrix) +
               "\" " + _texture->getAttributesAsString();
    }

}

