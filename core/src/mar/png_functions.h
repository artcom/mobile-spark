#ifndef _included_mobile_mar_png_functions_
#define _included_mobile_mar_png_functions_

#include <string>
#include <libzip/zip.h>
#include "openGL_functions.h"
#include "Material.h"

namespace mar {
    //Filename will be looked up in the apk (should start with assets/ or res/
    GLuint loadTextureFromPNG (zip* theAPKArchive, const std::string & filename, MaterialPtr theMaterial);
};

#endif 
