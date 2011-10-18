#ifndef _ac_mobile_mar_Texture_h_included_
#define _ac_mobile_mar_Texture_h_included_

#include <masl/Ptr.h>
#include "openGL_functions.h"

namespace mar {
    class TextureInfo {
        public:
            TextureInfo();
            ~TextureInfo();
            void unbind();

            GLuint width_;
            GLuint height_;
            bool transparency_;
            GLuint textureId_;        
        
    };
    typedef masl::Ptr<TextureInfo> TextureInfoPtr;
        
    class Texture {
        public:
            Texture();
            ~Texture();

            void setTextureInfo(TextureInfoPtr theTextureInfo);
            TextureInfoPtr getTextureInfo() { return _myTextureInfo;}
            matrix matrix_;
        private:
            TextureInfoPtr _myTextureInfo;
    };
    typedef masl::Ptr<Texture> TexturePtr;

};

#endif
