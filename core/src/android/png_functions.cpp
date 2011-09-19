#include "png_functions.h"

extern "C" {
#include <libpng/png.h>
}

#include <stdio.h>
#include <libzip/zip.h>

#include <masl/Logger.h>
#include <mar/png_functions.h>


namespace android {

zip* archive;
zip_file* file;

void
png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length) {
  zip_fread(file, data, length);
}

void
close() {
    zip_fclose(file);
}

bool
initFileReading(mar::pngData & thePngData) {
    AC_DEBUG << "-------------init file reading apk for " << thePngData.filename;
    file = zip_fopen(archive, thePngData.filename.c_str(), 0);
    if (!file) {
      AC_ERROR << "Error opening " << thePngData.filename << " from APK";
      return false;
    }
    //header for testing if it is a png
    png_byte header[8];
    //read the header
    zip_fread(file, header, 8);
    //test if png
    int is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png) {
      close();
      AC_ERROR << "Not a png file : " << thePngData.filename;
      return false;
    }
    return true;
}

void
prePNGReading(mar::pngData & thePngData) {
    png_set_read_fn(thePngData.png_ptr, NULL, png_zip_read);
    //let libpng know you already read the first 8 bytes
    png_set_sig_bytes(thePngData.png_ptr, 8);
    // read all the info up to the image data
    png_read_info(thePngData.png_ptr, thePngData.info_ptr);
}

bool
postPNGReading(mar::pngData & thePngData) {
    //row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep *row_pointers = new png_bytep[thePngData.theight];
    if (!row_pointers) {
      //clean up memory and close stuff
      png_destroy_read_struct(&thePngData.png_ptr, &thePngData.info_ptr, &thePngData.end_info);
      delete[] thePngData.image_data;
      AC_ERROR << "Unable to allocate row_pointer";
      close();
      return false;
    }
    // set the individual row_pointers to point at the correct offsets of image_data
    for (unsigned int i = 0; i < thePngData.theight; ++i)
      row_pointers[thePngData.theight - 1 - i] = thePngData.image_data + i * thePngData.row_bytes;

    //read the png into image_data through row_pointers
    png_read_image(thePngData.png_ptr, row_pointers);
    delete[] row_pointers;
    return true;
}

bool
loadTextureFromPNG(zip* theAPKArchive, const std::string & filename, GLuint & outTextureId, int & outWidth, int & outHeight, bool & outRgb) {
    archive = theAPKArchive;
    mar::pngData myData;
    return loadTextureFromPNGSkeleton(filename, outTextureId, outWidth, outHeight, outRgb, myData,
                               close, initFileReading, prePNGReading, postPNGReading);
}

} //namespace android
