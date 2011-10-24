#ifndef _included_mobile_android_png_functions_
#define _included_mobile_android_png_functions_

#include <string>
#include <libzip/zip.h>
#include <mar/GlHeaders.h>
#include <masl/Exception.h>

namespace android {
    DECLARE_EXCEPTION(PngAndroidLoadingException, masl::Exception)    
    bool loadTextureFromPNG (zip* theAPKArchive, const std::string & filename, GLuint & textureId, int & width, int & height, bool & rgb);
};

#endif
