
#include <masl/Logger.h>

#include "TextureLoader.h"
#include "png_functions.h"

namespace mar {
    
    TextureLoader::TextureLoader() {
    }

    TextureLoader::~TextureLoader() {}

    TexturePtr TextureLoader::load(const std::string & theSrc, const bool theCacheFlag) {
        if (_myTextureMap.find(theSrc) != _myTextureMap.end()) {
            AC_INFO << "TextureLoader::load found texture: '" << theSrc << "' in map -> do not reload";
            return _myTextureMap[theSrc];
        } else {
            TexturePtr myTexture = TexturePtr(new Texture());        
            loadTextureFromPNG(theSrc, myTexture);    
            if (theCacheFlag) {    
                AC_INFO << "TextureLoader::load texture: '" << theSrc << "' generated store in map";
                _myTextureMap[theSrc] = myTexture;
            }
            return myTexture;
        }
    }
}

