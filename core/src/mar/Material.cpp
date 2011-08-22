#include "Material.h"

#include <masl/Logger.h>
#include "openGL_functions.h"


namespace mar {
    Material::Material(const AssetProviderPtr theAssetProvider) : _myAssetProvider(theAssetProvider) {
    }

    Material::~Material() {
    }

    void Material::createShader() {
        std::string vertexShader = _myAssetProvider->getStringFromFile(DEFAULT_VERTEX_SHADER); 
        std::string fragmentShader = _myAssetProvider->getStringFromFile(DEFAULT_FRAGMENT_SHADER); 

        shaderProgram = createProgram(vertexShader.c_str(), fragmentShader.c_str());
        if (!shaderProgram) {
            AC_ERROR << "Could not create program.";
            return;
        }

        mvpHandle = glGetUniformLocation(shaderProgram, "u_mvpMatrix");
        colorHandle = glGetUniformLocation(shaderProgram, "a_color");
    }
}

