#ifndef _ac_mobile_mar_Texture_h_included_
#define _ac_mobile_mar_Texture_h_included_

#include <boost/enable_shared_from_this.hpp>
#include <masl/Ptr.h>
#include "openGL_functions.h"
#include "png_functions.h"

namespace mar {
    class Texture : public boost::enable_shared_from_this<Texture> {
        public:
            Texture();
            ~Texture();
            void unbind();
            std::string getSrc() const {return src_;};
            void setSrc(const std::string & theSrc = "");
            std::string getAttributesAsString() const;

            GLuint width_;
            GLuint height_;
            bool transparency_;
            GLuint textureId_;        
        private:
            std::string src_;
        
    };
    typedef masl::Ptr<Texture> TexturePtr;
        
};

#endif
