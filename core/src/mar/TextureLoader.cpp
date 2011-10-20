#include "TextureLoader.h"

#include <masl/Logger.h>
#include <masl/checksum.h>

#include "png_functions.h"

namespace mar {
    
    TextureLoader::TextureLoader() {
    }

    TextureLoader::~TextureLoader() {}
        
    void
    TextureLoader::clear() {
        _myTextureMap.clear();
    }
        
    TexturePtr TextureLoader::load(const std::string & theSrc, const bool theCacheFlag) {
        unsigned long myKey = masl::initiateCRC32();
        masl::appendCRC32(myKey, theSrc);     
        
        if (theCacheFlag && _myTextureMap.find(myKey) != _myTextureMap.end()) {
            AC_INFO << "TextureLoader::load found texture: '" << theSrc << "' in map -> do not reload, glid -> "<< _myTextureMap[myKey]->textureId_;
            return _myTextureMap[myKey];
        } else {
            TexturePtr myTexture = TexturePtr(new Texture());        
            loadTextureFromPNG(theSrc, myTexture);    
            if (theCacheFlag) {    
                AC_INFO << "TextureLoader::load texture: '" << theSrc << "' generated store in map, glid -> "<< myTexture->textureId_;
                storeTexture(myKey, myTexture);
            }
            return myTexture;
        }
    }
    
    TexturePtr 
    TextureLoader::getTexture(const unsigned long theKey) {
        if (_myTextureMap.find(theKey) != _myTextureMap.end()) {
            return _myTextureMap[theKey];
        } else {
            return TexturePtr();                    
        }
    }
        
    void 
    TextureLoader::storeTexture(const unsigned long theKey, TexturePtr theTexture) {
        _myTextureMap[theKey] = theTexture;
    }

}

