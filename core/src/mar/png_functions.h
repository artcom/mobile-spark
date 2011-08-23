#ifndef _included_mobile_mar_png_functions_
#define _included_mobile_mar_png_functions_

#include <string>
#include "openGL_functions.h"
#include "Material.h"

namespace mar {
    GLuint loadTextureFromPNG (AssetProviderPtr theAssetProvider, const std::string & filename, MaterialPtr theMaterial);
};

#endif 
