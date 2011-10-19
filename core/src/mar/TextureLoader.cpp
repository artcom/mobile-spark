
#include <masl/Logger.h>
#include <masl/checksum.h>

#include "TextureLoader.h"
#include "png_functions.h"

namespace mar {
    
    TextureLoader::TextureLoader() {
    }

    TextureLoader::~TextureLoader() {}
        
    void
    TextureLoader::clear() {
        _myTextureMap.clear();
    }
        
    TextureInfoPtr TextureLoader::load(const std::string & theSrc, const bool theCacheFlag) {
        unsigned long myKey = masl::initiateCRC32();
        masl::appendCRC32(myKey, theSrc);     
        
        if (_myTextureMap.find(myKey) != _myTextureMap.end()) {
            AC_INFO << "TextureLoader::load found texture: '" << theSrc << "' in map -> do not reload, glid -> "<< _myTextureMap[myKey]->textureId_;
            return _myTextureMap[myKey];
        } else {
            TextureInfoPtr myTexture = TextureInfoPtr(new TextureInfo());        
            loadTextureFromPNG(theSrc, myTexture);    
            if (theCacheFlag) {    
                AC_INFO << "TextureLoader::load texture: '" << theSrc << "' generated store in map, glid -> "<< myTexture->textureId_;
                storeTextureInfo(myKey, myTexture);
            }
            return myTexture;
        }
    }
    
    TextureInfoPtr 
    TextureLoader::getTextureInfo(const unsigned long theKey) {
        if (_myTextureMap.find(theKey) != _myTextureMap.end()) {
            return _myTextureMap[theKey];
        } else {
            return TextureInfoPtr();                    
        }
        
    }
        
    void 
    TextureLoader::storeTextureInfo(const unsigned long theKey, TextureInfoPtr theTextureInfo) {
        /*TextureInfoPtr myTexture = TextureInfoPtr(new TextureInfo());        
        myTexture->width_ = theTextureInfo->width_;
        myTexture->height_ = theTextureInfo->height_;
        myTexture->textureId_ = theTextureInfo->textureId_;
        myTexture->transparency_ = theTextureInfo->transparency_;*/
        _myTextureMap[theKey] = theTextureInfo;    
    }

}

