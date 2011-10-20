#include "TextureUnit.h"

#include <masl/Logger.h>
#include <masl/numeric_functions.h>

namespace mar {

    TextureUnit::TextureUnit() :matrix_(cml::identity_4x4()), _myTexture(TexturePtr(new Texture())) {
    }
    TextureUnit::TextureUnit(const TexturePtr theTexture) :
        matrix_(cml::identity_4x4()), _myTexture(theTexture)
    {}

    TextureUnit::~TextureUnit() {
    }
}

