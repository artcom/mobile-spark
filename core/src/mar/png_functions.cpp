#include "png_functions.h"

#include "AssetProvider.h"

namespace mar {

GLuint loadTextureFromPNG(const std::string & filename, UnlitTexturedMaterialPtr theMaterial) {

    theMaterial->textureFile = filename;
    GLuint textureId;
    int width, height;
    bool rgb;
    AssetProviderSingleton::get().ap()->loadTextureFromPNG(filename, textureId, width, height, rgb);

    theMaterial->textureId = textureId;
    theMaterial->rgb = rgb;
    theMaterial->width = width;
    theMaterial->height = height;

    return textureId;
}

} //namespace mar
