#include "png_functions.h"

#include "AssetProvider.h"

namespace mar {

GLuint loadTextureFromPNG(const std::string & filename, TexturePtr theTexture) {

    //theMaterial->textureFile = filename;
    GLuint textureId;
    int width, height;
    bool rgb;
    AssetProviderSingleton::get().ap()->loadTextureFromPNG(filename, textureId, width, height, rgb);
        
    theTexture->setTextureId(textureId);
    theTexture->transparency_ = !rgb;
    theTexture->width_ = width;
    theTexture->height_ = height;

    return textureId;
}

} //namespace mar
