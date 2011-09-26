#ifndef _included_mobile_mar_png_functions_
#define _included_mobile_mar_png_functions_

#include <string>
extern "C" {
#include <libpng/png.h>
}

#include <masl/Exception.h>

#include "openGL_functions.h"
#include "Material.h"

namespace mar {

    DEFINE_EXCEPTION(PngLoadingException, masl::Exception)

    struct pngData {
        std::string filename;
        png_structp png_ptr;
        png_infop info_ptr;
        png_infop end_info;
        png_uint_32 twidth;
        png_uint_32 theight;
        unsigned int row_bytes;
        GLubyte *image_data;
    };

    void loadTextureFromPNG (const std::string & filename, TexturePtr theTexture);
    bool loadTextureFromPNG(const std::string & filename, GLuint & textureId, int & outWidth, int & outHeight, bool & rgb);
    bool loadTextureFromPNGSkeleton(const std::string & filename, GLuint & outTextureId,
                                    int & outWidth, int & outHeight, bool & outRgb,
                                    pngData & thePngData,
                                    void (*close)(void),
                                    bool (*initFileReading)(pngData & thePngData),
                                    void (*prePNGReading)(pngData & thePngData),
                                    bool (*postPNGReading)(pngData & thePngData));
};

#endif
