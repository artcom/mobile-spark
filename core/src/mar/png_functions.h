#ifndef _included_mobile_mar_png_functions_
#define _included_mobile_mar_png_functions_

#include <string>
#include "openGL_functions.h"
#include "Material.h"

namespace mar {
    void loadTextureFromPNG (const std::string & filename, TexturePtr theTexture);
    bool loadTextureFromPNG(const std::string & filename, GLuint & textureId, int & outWidth, int & outHeight, bool & rgb);
};

#endif 
