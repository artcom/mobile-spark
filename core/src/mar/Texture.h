#ifndef _ac_mobile_masl_Texture_h_included_
#define _ac_mobile_masl_Texture_h_included_

#include <boost/smart_ptr/shared_ptr.hpp>

namespace mar {
        
    class Texture {
        public:
            Texture();
            GLuint getTextureId();
            void setTextureId(GLuint theTextureId);

            GLuint width;
            GLuint height;
            float transparency;            
        private:
            GLuint _myTextureId;                                    
    };
    typedef boost::shared_ptr<Texture> TexturePtr;
    
};

#endif 
