#include "Texture.h"
#include <masl/Logger.h>
#include <masl/numeric_functions.h>

namespace mar {
    Texture::Texture() :matrix_(cml::identity_4x4()), _myTextureInfo(TextureInfoPtr(new TextureInfo())) {
        AC_DEBUG << "create Texture " << (void*)this;        
    }

    Texture::~Texture() {
    }
    
    void 
    Texture::setTextureInfo(TextureInfoPtr theTextureInfo) {
        _myTextureInfo = theTextureInfo;
    }

    std::string
    Texture::getAttributesAsString() const {
        return (_myTextureInfo?_myTextureInfo->getAttributesAsString():"");
    }

    TextureInfo::TextureInfo() :textureId_(0) {
    }    
    TextureInfo::~TextureInfo() {
        AC_DEBUG << "delete Texture " << (void*)this << " texid: "<<textureId_;
        unbind();
    }
    
    void
    TextureInfo::unbind() {
        if (textureId_) {
            AC_DEBUG << "unbind gl texture " << textureId_;
            glDeleteTextures(1, &textureId_);
            textureId_ = 0;
        }
    }    

    std::string
    TextureInfo::getAttributesAsString() const {
        return "width=\""+masl::as_string(width_)+"\" height=\""+masl::as_string(height_)+"\""
            " transparency=\""+masl::as_string(transparency_)+"\""
            " textureId=\""+masl::as_string(textureId_)+"\"";
    }
}
