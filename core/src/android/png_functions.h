// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_android_png_functions_
#define _included_mobile_android_png_functions_

#include <string>
#include <libzip/zip.h>
#include <mar/GlHeaders.h>

namespace android {
    bool loadTextureFromPNG (zip* theAPKArchive, const std::string & filename, GLuint & textureId, int & width, int & height, bool & rgb);
};

#endif
