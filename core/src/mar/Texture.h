#ifndef _ac_mobile_masl_Texture_h_included_
#define _ac_mobile_masl_Texture_h_included_

#include <boost/smart_ptr/shared_ptr.hpp>
#include "openGL_functions.h"

namespace mar {
        
    class Texture {
        public:
            Texture();
            GLuint getTextureId() const ;
            void setTextureId(GLuint theTextureId);

            GLuint width_;
            GLuint height_;
            bool transparency_;            
        private:
            GLuint textureId_;
    };
    typedef boost::shared_ptr<Texture> TexturePtr;
    
};

#endif 
