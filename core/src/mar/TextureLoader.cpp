// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "TextureLoader.h"
#include "Texture.h"

#include <masl/Logger.h>
#include <masl/string_functions.h>
#include <masl/AssetProvider.h>

namespace mar {
    
    TextureLoader::TextureLoader() {
    }

    TextureLoader::~TextureLoader() {}
        
    void
    TextureLoader::clear() {
        _myTextureMap.clear();
    }
        
    TexturePtr
    TextureLoader::load(const std::string & theSrc, const bool theCacheFlag, const bool theMipmapFlag) {
        unsigned int myKey = masl::CRC32(theSrc);
        if (theCacheFlag && _myTextureMap.find(myKey) != _myTextureMap.end()) {
            AC_DEBUG << "TextureLoader::load found texture: '" << theSrc
                    << "' in map -> do not reload, glid -> "<< _myTextureMap[myKey]->_textureId;
            return _myTextureMap[myKey];
        } else {
            TexturePtr myTexture = TexturePtr(new Texture());       
            myTexture->_mipmapFlag = theMipmapFlag;
        
            masl::AssetProviderSingleton::get().ap()->loadTextureFromFile(theSrc, myTexture->_textureId, 
                                                                          myTexture->_width, myTexture->_height,  
                                                                          myTexture->_real_width, myTexture->_real_height,
                                                                          myTexture->getNpotMatrix(),
                                                                          myTexture->_transparency,
                                                                          myTexture->_mirrorFlag,
                                                                          myTexture->_mipmapFlag);                        
            if (theCacheFlag) {    
                AC_DEBUG << "TextureLoader::load texture: '" << theSrc
                        << "' generated store in map, glid -> "<< myTexture->_textureId;
                storeTexture(myKey, myTexture);
            }
            return myTexture;
        }
    }
    
    TexturePtr 
    TextureLoader::getTexture(const unsigned int theKey) {
        if (_myTextureMap.find(theKey) != _myTextureMap.end()) {
            AC_DEBUG << "TextureLoader::getTexture : '" << theKey
                    << "' in map -> do not reload, glid -> "<< _myTextureMap[theKey]->_textureId;
            return _myTextureMap[theKey];
        } else {
            return TexturePtr();                    
        }
    }
        
    void 
    TextureLoader::storeTexture(const unsigned int theKey, TexturePtr theTexture) {
        AC_DEBUG << "TextureLoader::storeTexture : '" << theKey
                << "' generated store in map, glid -> "<< theTexture->_textureId;
        _myTextureMap[theKey] = theTexture;
    }

}

