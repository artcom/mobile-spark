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
    AssetProviderSingleton::get().ap()->loadTextureFromPNG(filename, textureId, width, height, rgb);
        
    theTexture->setTextureId(textureId);
    theTexture->transparency_ = !rgb;
    theTexture->width_ = width;
    theTexture->height_ = height;
}

GLuint loadTextureFromPNG(const std::string & filename, GLuint & textureId, int & outWidth, int & outHeight, bool & rgb) {
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    GLubyte *outData;
    std::string filepath = filename;
    FILE *fp;
    if ((fp = fopen(filepath.c_str(), "rb")) == NULL) {
        AC_PRINT << " Error opening ";
        return false;   
    }
    
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
         
    if (png_ptr == NULL) {
        fclose(fp);
        return false;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return false;
    }
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, sig_read);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);


    // get info about png
    int bit_depth, color_type;
    png_uint_32 twidth, theight;
    png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
           NULL, NULL, NULL);
    outWidth = twidth;
    outHeight = theight;
  
    switch (color_type) {
        case PNG_COLOR_TYPE_RGBA:
            rgb = !true;
            break;
        case PNG_COLOR_TYPE_RGB:
            rgb = !false;
            break;
        default:
            std::cout << "Color type " << color_type << " not supported" << std::endl;
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            return false;
    }
    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    outData = (unsigned char*) malloc(row_bytes * outHeight);
 
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
    for (int i = 0; i < outHeight; i++) {
        memcpy(outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
    }
    
    
    //Now generate the OpenGL texture object
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    AC_PRINT << "w x h : " << outWidth << ", " << outHeight;
    if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
        AC_PRINT << "alpha";
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, outWidth, outHeight, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, (GLvoid*) outData);
        //rgb = false;
    } else if (color_type == PNG_COLOR_TYPE_RGB) {
        AC_PRINT << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>> no alpha";
        //glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, outWidth, outHeight, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, (GLvoid*) outData);
        //rgb = true;
    } else {
        AC_PRINT << "unknown color type " << color_type;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    AC_PRINT << "generated texture with id " << texture;
    
    //clean up memory and close stuff
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    delete[] outData;
    textureId = texture;
    fclose(fp);
    return true;
}

} //namespace mar
