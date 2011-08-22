#ifndef _included_mobile_mar_png_functions_
#define _included_mobile_mar_png_functions_

#include <libzip/zip.h>
#include "openGL_functions.h"

namespace mar {
    //Filename will be looked up in the apk (should start with assets/ or res/
    GLuint loadTextureFromPNG (zip* theAPKArchive, const char* filename, int &width, int &height, bool &rgb);
};

#endif 
