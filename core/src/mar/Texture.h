#ifndef _ac_mobile_mar_Texture_h_included_
#define _ac_mobile_mar_Texture_h_included_

#include <boost/enable_shared_from_this.hpp>
#include <masl/Ptr.h>
#include "openGL_functions.h"
#include "png_functions.h"

namespace mar {
    class TextureInfo : public boost::enable_shared_from_this<TextureInfo> {
        public:
            TextureInfo();
            ~TextureInfo();
            void unbind();
            std::string getSrc() const {return src_;};
            void setSrc(const std::string & theSrc);

            GLuint width_;
            GLuint height_;
            bool transparency_;
            GLuint textureId_;        
            std::string src_;
        
    };
    typedef masl::Ptr<TextureInfo> TextureInfoPtr;
        
    class Texture {
        public:
            Texture();
            ~Texture();

            void setTextureInfo(TextureInfoPtr theTextureInfo) {_myTextureInfo = theTextureInfo;};
            TextureInfoPtr getTextureInfo() const { return _myTextureInfo;}
            matrix matrix_;
        private:
            TextureInfoPtr _myTextureInfo;
    };
    typedef masl::Ptr<Texture> TexturePtr;

};

#endif
