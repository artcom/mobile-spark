#ifndef _ac_mobile_mar_Textureunit_h_included_
#define _ac_mobile_mar_Textureunit_h_included_

#include <masl/Ptr.h>

#include "Texture.h"

namespace mar {
        
    class TextureUnit {
        public:
            TextureUnit();
            TextureUnit(const TexturePtr theTexture);
            ~TextureUnit();

            void setTexture(TexturePtr theTexture) {_myTexture = theTexture;};
            TexturePtr getTexture() const { return _myTexture;}
            matrix matrix_;
        private:
            TexturePtr _myTexture;
    };
    typedef masl::Ptr<TextureUnit> TextureUnitPtr;

};

#endif

