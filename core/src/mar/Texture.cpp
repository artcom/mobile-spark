#include "Texture.h"
#include <masl/Logger.h>
#include <masl/numeric_functions.h>

namespace mar {

    TextureInfo::TextureInfo() :
        width_(0), height_(0), transparency_(false),
        textureId_(0) 
    {
        AC_DEBUG << "create Texture " << (void*)this;
    }

    TextureInfo::~TextureInfo() {
        AC_DEBUG << "delete Texture " << (void*)this << " texid: "<<textureId_;
        unbind();
    }

    void
    TextureInfo::setSrc(const std::string & theSrc) {
        AC_DEBUG << "setSrc " << theSrc << " for Texture: " << (void*)this;
        src_ = theSrc;
        unbind();
        if (!src_.empty()) {
            loadTextureFromPNG(src_, shared_from_this());
        } else {
            width_ = 0;
            height_ = 0;
            transparency_ = false;
        }
    }

    void
    TextureInfo::unbind() {
        if (textureId_) {
            AC_DEBUG << "unbind gl texture " << textureId_;
            glDeleteTextures(1, &textureId_);
            textureId_ = 0;
        }
    }    
    
    Texture::Texture() :matrix_(cml::identity_4x4()), _myTextureInfo(TextureInfoPtr(new TextureInfo())) {
    }

    Texture::~Texture() {
    }
}
