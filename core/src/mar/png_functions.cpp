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
    theTexture->transparency = !rgb;
    theTexture->width = width;
    theTexture->height = height;

    return textureId;
}

} //namespace mar
