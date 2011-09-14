#include "png_functions.h"

#include "AssetProvider.h"

extern "C" {
#include <libpng/png.h>
}


namespace mar {

void loadTextureFromPNG(const std::string & filename, TexturePtr theTexture) {

    GLuint textureId;
    int width, height;
    bool rgb;
    if (AssetProviderSingleton::get().ap()->loadTextureFromPNG(filename, textureId, width, height, rgb)) {
        theTexture->setTextureId(textureId);
        theTexture->transparency_ = !rgb;
        theTexture->width_ = width;
        theTexture->height_ = height;
    }
}

bool loadTextureFromPNG(const std::string & filename, GLuint & outTextureId, int & outWidth, int & outHeight, bool & outRgb) {
    std::string myFilename = masl::trimall(filename);
    AC_PRINT << "load texture file name '" << filename << "'";

    ///////////////////////////////////////////////////
    FILE *file;
    if ((file = fopen(myFilename.c_str(), "rb")) == NULL) {
        AC_PRINT << " Error opening ";
        return false;   
    }
    ///////////////////////////////////////////////////
    
    //create png struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(file);
        AC_ERROR << "Unable to create png struct : " << myFilename;
        return false;
    }

    AC_PRINT << "1";
    //create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        AC_ERROR << "Unable to create png info : " << myFilename;
        fclose(file);
        return false;
    }

    AC_PRINT << "2";
    //create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      AC_ERROR << "Unable to create png end info : " << myFilename;
      fclose(file);
      return false;
    }

    AC_PRINT << "3";
    //png error stuff, not sure libpng man suggests this.
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        AC_ERROR << "Error during setjmp : " << myFilename;
        fclose(file);
        return false;
    }

    AC_PRINT << "4";
    unsigned int sig_read = 0;
    GLubyte *image_data;
    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, sig_read);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);


    AC_PRINT << "5";
    // get info about png
    int bit_depth, color_type;
    png_uint_32 twidth, theight;
    png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type, NULL, NULL, NULL);
  
    if (color_type != PNG_COLOR_TYPE_RGB_ALPHA && color_type != PNG_COLOR_TYPE_RGB) {
            AC_DEBUG << "unknown color type " << color_type;
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(file);
            return false;
    }
    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    image_data = (unsigned char*) malloc(row_bytes * outHeight);
 
    AC_PRINT << "6";
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
    for (int i = 0; i < outHeight; i++) {
        memcpy(image_data+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
    }
    
    AC_PRINT << "7";
    
    //Now generate the OpenGL texture object
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    AC_DEBUG << "w x h : " << twidth << ", " << theight;
    if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
        AC_DEBUG << "alpha";
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, twidth, theight, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, (GLvoid*) image_data);
        outRgb = false;
    } else if (color_type == PNG_COLOR_TYPE_RGB) {
        AC_DEBUG << "no alpha";
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, outWidth, outHeight, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, (GLvoid*) image_data);
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
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    delete[] image_data;
    fclose(file);

    outTextureId = texture;
    outWidth = twidth;
    outHeight = theight;
    return true;
}

} //namespace mar
