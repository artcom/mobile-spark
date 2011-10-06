#include "Texture.h"
#include <masl/Logger.h>

namespace mar {
    Texture::Texture() :textureId_(0){
    }

    Texture::~Texture() {
        AC_DEBUG << "delete Texture " << (void*)this;
        unbind();
    }

    GLuint
    Texture::getTextureId() const {
        return textureId_;
    }

    void
    Texture::setTextureId(GLuint theTextureId){
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
