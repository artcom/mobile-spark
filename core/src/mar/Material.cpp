#include "Material.h"

#include <masl/Logger.h>
#include "openGL_functions.h"


namespace mar {
    Material::Material(const AssetProviderPtr theAssetProvider) 
        : _myAssetProvider(theAssetProvider) {
    }
    Material::~Material() {
    }
    void Material::createShader() {
        setShader();
        shaderProgram = createProgram(_myVertexShader, _myFragmentShader);
        if (!shaderProgram) {
            AC_ERROR << "Could not create program.";
            return;
        }
        setHandles();
    }
    void Material::setShader() {
        _myVertexShader = _myAssetProvider->getStringFromFile(DEFAULT_VERTEX_SHADER); 
    }
    void Material::setHandles() {
        mvpHandle = glGetUniformLocation(shaderProgram, "u_mvpMatrix");
    }

    UnlitColoredMaterial::UnlitColoredMaterial(const AssetProviderPtr theAssetProvider) : Material(theAssetProvider) {
    }
    UnlitColoredMaterial::~UnlitColoredMaterial() {
    }
    void UnlitColoredMaterial::setShader() {
        Material::setShader();
        _myFragmentShader = _myAssetProvider->getStringFromFile(DEFAULT_COLORED_FRAGMENT_SHADER); 
    }
    void UnlitColoredMaterial::setHandles() {
        Material::setHandles();
        colorHandle = glGetUniformLocation(shaderProgram, "a_color");
    }

    UnlitTexturedMaterial::UnlitTexturedMaterial(const AssetProviderPtr theAssetProvider) : Material(theAssetProvider) {
    }
    UnlitTexturedMaterial::~UnlitTexturedMaterial() {
    }
    void UnlitTexturedMaterial::setShader() {
        Material::setShader();
        _myFragmentShader = _myAssetProvider->getStringFromFile(DEFAULT_TEXTURED_FRAGMENT_SHADER); 
    }
}

