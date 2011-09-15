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

FILE* file;
png_structp png_ptr;
png_infop info_ptr;
png_infop end_info;
png_uint_32 twidth;
png_uint_32 theight;
unsigned int row_bytes;
GLubyte *image_data;

void 
close() {
    fclose(file);
}

bool 
initFileReading(const std::string & theFile) {
    if ((file = fopen(theFile.c_str(), "rb")) == NULL) {
        AC_ERROR << " Error opening file " << theFile;
        return false;   
    }
    return true;
}

void
prePNGReading() {
    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, 0);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
}

bool
postPNGReading() {
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
    for (size_t i = 0; i < theight; i++) {
        memcpy(image_data+(row_bytes * (theight-1-i)), row_pointers[i], row_bytes);
    }
    return true;
}

bool 
loadTextureFromPNG(const std::string & filename, GLuint & outTextureId, int & outWidth, int & outHeight, bool & outRgb) {
    std::string myFilename = masl::trimall(filename);
    AC_DEBUG << "load texture file name '" << myFilename << "'";
    if (!initFileReading(myFilename)) {
        return false;
    }
    
    //create png struct
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        close();
        AC_ERROR << "Unable to create png struct : " << myFilename;
        return false;
    }

    //create png info struct
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        close();
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        AC_ERROR << "Unable to create png info : " << myFilename;
        return false;
    }
    end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
      close();
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      AC_ERROR << "Unable to create png end info : " << myFilename;
      return false;
    }

    //png error stuff, not sure libpng man suggests this.
    if (setjmp(png_jmpbuf(png_ptr))) {
        close();
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        AC_ERROR << "Error during setjmp : " << myFilename;
        return false;
    }

    prePNGReading();

    // get info about png
    int bit_depth, color_type;
    png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type, NULL, NULL, NULL);
  
    // Allocate the image_data as a big block, to be given to opengl
    row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    image_data = new GLubyte[row_bytes * theight];
    if (!image_data) {
      //clean up memory and close stuff
      png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
      AC_ERROR << "Unable to allocate image_data while loading " << myFilename;
      close();
      return false;
    }
 
    if (!postPNGReading()) {
        return false;
    }
    
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB,
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
    close();

    outTextureId = texture;
    outWidth = twidth;
    outHeight = theight;
    return true;
}

} //namespace mar
