#include "Texture.h"
#include <masl/Logger.h>
#include <masl/numeric_functions.h>

namespace mar {

    Texture::Texture() :
        width_(0), height_(0), transparency_(false),
        textureId_(0) 
    {
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
    Texture::unbind() {
        if (textureId_) {
            AC_DEBUG << "unbind gl texture " << textureId_;
            glDeleteTextures(1, &textureId_);
            textureId_ = 0;
        }
    }    

    std::string
    Texture::getAttributesAsString() const {
        return "src=\""+src_+"\" width=\""+masl::as_string(width_)+"\" height=\""+masl::as_string(height_)+"\""
            " transparency=\""+masl::as_string(transparency_)+"\""
            " textureId=\""+masl::as_string(textureId_)+"\"";
    }
}
