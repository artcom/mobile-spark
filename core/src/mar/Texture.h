#ifndef _ac_mobile_mar_Texture_h_included_
#define _ac_mobile_mar_Texture_h_included_

#include <masl/Ptr.h>
#include "openGL_functions.h"

namespace mar {

    class Texture {
        public:
            Texture();
            ~Texture();
            void unbind();
            GLuint getTextureId() const;
            void setTextureId(GLuint theTextureId);

            GLuint width_;
            GLuint height_;
            bool transparency_;
            matrix matrix_;
        private:
            GLuint textureId_;
    };
    typedef masl::Ptr<Texture> TexturePtr;

};

#endif
