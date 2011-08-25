#ifndef _included_mobile_mar_png_functions_
#define _included_mobile_mar_png_functions_

#include <string>
#include "openGL_functions.h"
#include "Material.h"

namespace mar {
    GLuint loadTextureFromPNG (const std::string & filename, UnlitTexturedMaterialPtr theMaterial);
};

#endif 
