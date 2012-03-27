// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_mar_Textureunit_h_included_
#define _ac_mobile_mar_Textureunit_h_included_

#include <masl/Ptr.h>

#include "Texture.h"

namespace mar {

    class TextureUnit {
        public:
            TextureUnit();
            TextureUnit(const TexturePtr theTexture);
            ~TextureUnit();

            std::string getAttributesAsString() const;
            void setTexture(TexturePtr theTexture) { _texture = theTexture;};
            const TexturePtr getTexture() const { return _texture;}
            matrix & getMatrix() { return _matrix;};
            const matrix & getMatrix() const { return _matrix;};
            matrix getRenderMatrix() const;
        private:
            TexturePtr _texture;
            matrix _matrix;
    };
    typedef masl::Ptr<TextureUnit> TextureUnitPtr;

};

#endif

