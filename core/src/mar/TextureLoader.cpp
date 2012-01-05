// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "TextureLoader.h"

#include <masl/Logger.h>
#include <masl/checksum.h>
#include <masl/AssetProvider.h>

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
            masl::AssetProviderSingleton::get().ap()->loadTextureFromFile(theSrc, myTexture->textureId_, 
                                                                          myTexture->width_, myTexture->height_,  
                                                                          myTexture->transparency_);                        
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
            AC_INFO << "TextureLoader::getTexture : '" << theKey << "' in map -> do not reload, glid -> "<< _myTextureMap[theKey]->textureId_;
            return _myTextureMap[theKey];
        } else {
            return TexturePtr();                    
        }
    }
        
    void 
    TextureLoader::storeTexture(const unsigned long theKey, TexturePtr theTexture) {
        AC_INFO << "TextureLoader::storeTexture : '" << theKey << "' generated store in map, glid -> "<< theTexture->textureId_;
        _myTextureMap[theKey] = theTexture;
    }

}

