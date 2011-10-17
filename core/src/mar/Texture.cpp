#include "Texture.h"
#include <masl/Logger.h>
#include <masl/numeric_functions.h>

namespace mar {
    Texture::Texture() :matrix_(cml::identity_4x4()), textureId_(0) {
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
        loadTextureFromPNG(src_, shared_from_this());
    }
    void
    Texture::setTextureId(GLuint theTextureId) {
        AC_DEBUG << "setTexture " << theTextureId << " for Texture: " << (void*)this;
        textureId_ = theTextureId;
    }

    void
    Texture::unbind() {
        if (textureId_) {
            AC_DEBUG << "unbind gl texture " << textureId_;
            glDeleteTextures(1, &textureId_);
            textureId_ = 0;
        }
    }
}
