#include "IOSAssetProvider.h"

extern "C" {
#include <libpng/png.h>
}


namespace ios 
{
    IOSAssetProvider::IOSAssetProvider(const std::string & theAssetFolderPath)
    : AssetProvider(), _myAssetFolderPath(theAssetFolderPath)
    {
    }

    IOSAssetProvider::~IOSAssetProvider()
    {
    }

    bool IOSAssetProvider::loadTextureFromPNG(const std::string & filename, GLuint & textureId, int & outWidth, int & outHeight, bool & rgb)
    {
        png_structp png_ptr;
        png_infop info_ptr;
        unsigned int sig_read = 0;
        int color_type, interlace_type;
        GLubyte *outData;
        std::string filepath =  _myAssetFolderPath + "/" + filename;
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
        outWidth = info_ptr->width;
        outHeight = info_ptr->height;
        switch (info_ptr->color_type) {
	        case PNG_COLOR_TYPE_RGBA:
	            rgb = !true;
	            break;
	        case PNG_COLOR_TYPE_RGB:
	            rgb = !false;
	            break;
	        default:
	            std::cout << "Color type " << info_ptr->color_type << " not supported" << std::endl;
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
        if (info_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
            AC_PRINT << "alpha";
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, outWidth, outHeight, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, (GLvoid*) outData);
            //rgb = false;
        } else if (info_ptr->color_type == PNG_COLOR_TYPE_RGB) {
            AC_PRINT << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>> no alpha";
            //glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, outWidth, outHeight, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, (GLvoid*) outData);
            //rgb = true;
        } else {
            AC_PRINT << "unknown color type " << info_ptr->color_type;
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
    
    
    std::string IOSAssetProvider::getStringFromFile(const std::string & theFile) const 
    {
        std::string content = "";
        masl::readFile(_myAssetFolderPath + "/" + theFile, content);
       
        return content;
    }


    std::vector<std::string> IOSAssetProvider::getLineByLineFromFile(const std::string & theFile) const
    {
        std::vector<std::string> myLines;
        std::vector<std::string> content;
        const size_t MAX_LENGTH = 1000;
        char buffer[MAX_LENGTH];
        std::string newPart;
        std::string filepath =  _myAssetFolderPath + "/" + theFile;
        FILE *file;
        if ((file = fopen(filepath.c_str(), "rb")) == NULL) {
            AC_PRINT << " Error opening ";
            return content;
        }
        size_t size = std::fread(buffer, 1, MAX_LENGTH,file);
        bool endedWithNewLine = false;
        while (size > 0) {
            newPart = std::string(buffer, size);
            std::stringstream stream(newPart);
            std::string item;
            bool first = true;
            while (std::getline(stream, item, '\n')) {
                if (first && !endedWithNewLine && content.size() >0) {
                    content.back().append(item);
                } else {
                    content.push_back(item);
                }
                first = false;
            }
            endedWithNewLine = (item.size() == 0);
            size = std::fread(buffer, 1, MAX_LENGTH,file);
        }
        fclose(file);
        return content;
    }
}
