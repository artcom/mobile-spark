#include "Texture.h"

namespace mar {
    Texture::Texture() :textureId_(0){
    }

    GLuint Texture::getTextureId() const {
        return textureId_;
    }
    void Texture::setTextureId(GLuint theTextureId){
        textureId_ = theTextureId;
    }

}
