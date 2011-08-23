#include "png_functions.h"

namespace mar {

GLuint loadTextureFromPNG(AssetProviderPtr theAssetProvider, const std::string & filename, UnlitTexturedMaterialPtr theMaterial) {

    theMaterial->textureFile = filename;
    GLuint textureId;
    int width, height;
    bool rgb;
    theAssetProvider->loadTextureFromPNG(filename, textureId, width, height, rgb);

    theMaterial->textureId = textureId;
    theMaterial->rgb = rgb;
    theMaterial->width = width;
    theMaterial->height = height;

    return textureId;
}

} //namespace mar
