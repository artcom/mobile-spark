#include "png_functions.h"

#include <masl/AssetProvider.h>
#include "Texture.h"


namespace mar {

DEFINE_EXCEPTION(PngLoadingException, masl::Exception)

void loadTextureFromPNG(const std::string & filename, TexturePtr theTexture) {
    GLuint textureId;
    int width, height;
    bool rgb;
    if (masl::AssetProviderSingleton::get().ap()->loadTextureFromPNG(filename, textureId, width, height, rgb)) {
        theTexture->textureId_ = textureId;//setTextureId(textureId);
        theTexture->transparency_ = !rgb;
        theTexture->width_ = width;
        theTexture->height_ = height;
    }
}

FILE* file;

void
close() {
    fclose(file);
}

bool
initFileReading(pngData & thePngData) {
    AC_DEBUG << "--init file reading for " << thePngData.filename;
    if ((file = fopen(thePngData.filename.c_str(), "rb")) == NULL) {
        AC_ERROR << " Error opening file " << thePngData.filename;
        throw PngLoadingException("Error opening file " +thePngData.filename, PLUS_FILE_LINE);
        return false;
    }
    return true;
}

void
prePNGReading(pngData & thePngData) {
    png_init_io(thePngData.png_ptr, file);
    png_set_sig_bytes(thePngData.png_ptr, 0);
    png_read_png(thePngData.png_ptr, thePngData.info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
}

bool
postPNGReading(pngData & thePngData) {
    png_bytepp row_pointers = png_get_rows(thePngData.png_ptr, thePngData.info_ptr);
    for (size_t i = 0; i < thePngData.theight; i++) {
        memcpy(thePngData.image_data+(thePngData.row_bytes * (thePngData.theight-1-i)),
                row_pointers[i], thePngData.row_bytes);
    }
    return true;
}


bool
loadTextureFromPNGSkeleton(const std::string & filename, GLuint & outTextureId,
                           int & outWidth, int & outHeight, bool & outRgb,
                           pngData & thePngData,
                           void (*close)(void),
                           bool (*initFileReading)(pngData & thePngData),
                           void (*prePNGReading)(pngData & thePngData),
                           bool (*postPNGReading)(pngData & thePngData)) {
    thePngData.filename = masl::trimall(filename);
    AC_DEBUG << "load texture file name '" << thePngData.filename << "'";

    if (!initFileReading(thePngData)) {
        return false;
    }

    //create png struct
    thePngData.png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!thePngData.png_ptr) {
        close();
        AC_ERROR << "Unable to create png struct : " << thePngData.filename;
        throw PngLoadingException("unable to create png struct " +thePngData.filename, PLUS_FILE_LINE);
        return false;
    }

    //create png info struct
    thePngData.info_ptr = png_create_info_struct(thePngData.png_ptr);
    if (!thePngData.info_ptr) {
        close();
        png_destroy_read_struct(&thePngData.png_ptr, NULL, NULL);
        AC_ERROR << "Unable to create png info : " << thePngData.filename;
        throw PngLoadingException("Unable to create png info " +thePngData.filename, PLUS_FILE_LINE);
        return false;
    }
    thePngData.end_info = png_create_info_struct(thePngData.png_ptr);
    if (!thePngData.end_info) {
      close();
      png_destroy_read_struct(&thePngData.png_ptr, &thePngData.info_ptr, NULL);
        throw PngLoadingException("Unable to create png end info " +thePngData.filename, PLUS_FILE_LINE);
      AC_ERROR << "Unable to create png end info : " << thePngData.filename;
      return false;
    }

    //png error stuff, not sure libpng man suggests this.
    if (setjmp(png_jmpbuf(thePngData.png_ptr))) {
        close();
        png_destroy_read_struct(&thePngData.png_ptr, &thePngData.info_ptr, &thePngData.end_info);
        throw PngLoadingException("Error during setjmp " +thePngData.filename, PLUS_FILE_LINE);
        AC_ERROR << "Error during setjmp : " << thePngData.filename;
        return false;
    }

    prePNGReading(thePngData);

    // get info about png
    int bit_depth, color_type;
    png_get_IHDR(thePngData.png_ptr, thePngData.info_ptr, &thePngData.twidth, &thePngData.theight,
                 &bit_depth, &color_type, NULL, NULL, NULL);

    // Allocate the image_data as a big block, to be given to opengl
    thePngData.row_bytes = png_get_rowbytes(thePngData.png_ptr, thePngData.info_ptr);
    thePngData.image_data = new GLubyte[thePngData.row_bytes * thePngData.theight];
    if (!thePngData.image_data) {
        //clean up memory and close stuff
        png_destroy_read_struct(&thePngData.png_ptr, &thePngData.info_ptr, &thePngData.end_info);
        close();
        AC_ERROR << "Unable to allocate image_data while loading " << thePngData.filename;
        throw PngLoadingException("Unable to allocate image_data while loading " +thePngData.filename, PLUS_FILE_LINE);
        return false;
    }

    if (!postPNGReading(thePngData)) {
        return false;
    }

    //Now generate the OpenGL texture object
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    AC_DEBUG << "w x h : " << thePngData.twidth << ", " << thePngData.theight;
    if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
        AC_DEBUG << "alpha";
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, thePngData.twidth, thePngData.theight, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, (GLvoid*) thePngData.image_data);
        outRgb = false;
    } else if (color_type == PNG_COLOR_TYPE_RGB) {
        AC_DEBUG << "no alpha";
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, thePngData.twidth, thePngData.theight, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, (GLvoid*) thePngData.image_data);
        outRgb = true;
    } else {
        AC_DEBUG << "unknown color type " << color_type;
    }

    // http://www.khronos.org/webgl/wiki/WebGL_and_OpenGL_Differences#Non-Power_of_Two_Texture_Support
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    AC_DEBUG << "generated texture with id " << texture;

    //clean up memory and close stuff
    png_destroy_read_struct(&thePngData.png_ptr, &thePngData.info_ptr, &thePngData.end_info);
    delete[] thePngData.image_data;
    close();

    outTextureId = texture;
    outWidth = thePngData.twidth;
    outHeight = thePngData.theight;
    return true;
}

bool
loadTextureFromPNG(const std::string & filename, GLuint & outTextureId, int & outWidth, int & outHeight, bool & outRgb) {
    pngData myData;
    return loadTextureFromPNGSkeleton(filename, outTextureId, outWidth, outHeight, outRgb, myData,
            close, initFileReading, prePNGReading, postPNGReading);
}

} //namespace mar
