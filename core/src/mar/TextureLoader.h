#ifndef _ac_mobile_masl_TextureLoader_h_included_
#define _ac_mobile_masl_TextureLoader_h_included_

#include <masl/Singleton.h>
#include "Texture.h"

namespace mar {
    typedef std::map<std::string, TexturePtr> TextureMap;
    class TextureLoader : public masl::Singleton<TextureLoader> {
        public:
            TextureLoader();
            virtual ~TextureLoader();
            // we want a real singleton -> we need a non purevirtual out-of-line method other than dtor
            virtual void clear();
            TexturePtr load(const std::string & theSrc, const bool theCacheFlag = false);            
        private:
            TextureMap _myTextureMap;
    };
}
#endif
