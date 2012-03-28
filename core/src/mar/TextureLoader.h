// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_masl_TextureLoader_h_included_
#define _ac_mobile_masl_TextureLoader_h_included_

#include <masl/Singleton.h>
#include "Texture.h"

namespace mar {
    typedef std::map<unsigned int, TexturePtr> TextureMap;

    class TextureLoader : public masl::Singleton<TextureLoader> {
        public:
            TextureLoader();
            virtual ~TextureLoader();
            // we want a real singleton -> we need a non purevirtual out-of-line method other than dtor
            virtual void clear();
            TexturePtr load(const std::string & theSrc, const bool theCacheFlag = false, const bool theMipmapFlag = false);
            void storeTexture(const unsigned int theKey, TexturePtr theTexture);
            TexturePtr getTexture(const unsigned int theKey);
        private:
            TextureMap _myTextureMap;
    };
}
#endif
