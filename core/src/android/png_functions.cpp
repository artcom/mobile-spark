#include "png_functions.h"

extern "C" {
#include <libpng/png.h>
}

#include <stdio.h>
#include <libzip/zip.h>

#include <masl/Logger.h>


namespace android {

zip_file* file;

void png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length) {
  zip_fread(file, data, length);
}

bool loadTextureFromPNG(zip* theAPKArchive, const std::string & filename, GLuint & outTextureId, int & outWidth, int & outHeight, bool & outRgb) {
    std::string myFilename = masl::trimall(filename);
    AC_PRINT << "load texture file name ' " << myFilename << "'";
  
    ///////////////////////////////////////////////////////////////
    file = zip_fopen(theAPKArchive, myFilename.c_str(), 0);
    if (!file) {
      AC_ERROR << "Error opening " << myFilename << " from APK";
      return false;
    }
  
    //header for testing if it is a png
    png_byte header[8];
  
    //read the header
    zip_fread(file, header, 8);
    AC_PRINT << header;
  
    //test if png
    int is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png) {
      zip_fclose(file);
      AC_ERROR << "Not a png file : " << myFilename;
      return false;
    }
    /////////////////////////////////////////////////////////////
  
    //create png struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
      zip_fclose(file);
      AC_ERROR << "Unable to create png struct : " << myFilename;
      return false;
    }
  
    //create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
      png_destroy_read_struct(&png_ptr, NULL, NULL);
      AC_ERROR << "Unable to create png info : " << myFilename;
      zip_fclose(file);
      return false;
    }
  
    //create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      AC_ERROR << "Unable to create png end info : " << myFilename;
      zip_fclose(file);
      return (false);
    }
  
    //png error stuff, not sure libpng man suggests this.
    if (setjmp(png_jmpbuf(png_ptr))) {
      png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
      AC_ERROR << "Error during setjmp : " << myFilename;
      zip_fclose(file);
      return (false);
    }
  
    /////////////////////////////////////////////////////
    //init png reading
    png_set_read_fn(png_ptr, NULL, png_zip_read);
  
    //let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);
  
    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);
  
    // get info about png
    int bit_depth, color_type;
    png_uint_32 twidth, theight;
    png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type, NULL, NULL, NULL);
  
    // Allocate the image_data as a big block, to be given to opengl
    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    GLubyte *image_data = new GLubyte[row_bytes * theight];
    if (!image_data) {
      //clean up memory and close stuff
      png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
      AC_ERROR << "Unable to allocate image_data while loading " << myFilename;
      zip_fclose(file);
      return false;
    }
  
    //row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep *row_pointers = new png_bytep[theight];
    if (!row_pointers) {
      //clean up memory and close stuff
      png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
      delete[] image_data;
      AC_ERROR << "Unable to allocate row_pointer while loading " << myFilename;
      zip_fclose(file);
      return false;
    }
    // set the individual row_pointers to point at the correct offsets of image_data
    for (unsigned int i = 0; i < theight; ++i)
      row_pointers[theight - 1 - i] = image_data + i * row_bytes;
  
    //read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);
    /////////////////////////////////////////////////////
  
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
    delete[] row_pointers;
    zip_fclose(file);
  
    outTextureId = texture;
    outWidth = twidth;
    outHeight = theight;
    return true;
}

} //namespace android
