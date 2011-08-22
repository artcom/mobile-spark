#include "Material.h"

#include <masl/Logger.h>
#include "openGL_functions.h"


namespace mar {
    Material::Material(const AssetProviderPtr theAssetProvider, GLuint theMaterialMode) 
        : _myMaterialMode(theMaterialMode), _myAssetProvider(theAssetProvider) {
    }

    Material::~Material() {
    }

    void Material::createShader() {
        std::string vertexShader, fragmentShader;

        vertexShader = _myAssetProvider->getStringFromFile(DEFAULT_VERTEX_SHADER); 

        //TODO: material inheritance?
        if (_myMaterialMode == UNLIT_COLORED_MATERIAL) {
            fragmentShader = _myAssetProvider->getStringFromFile(DEFAULT_FRAGMENT_SHADER); 
        } else if (_myMaterialMode ==  UNLIT_TEXTURED_MATERIAL) {
            AC_PRINT << "............... create textured material";
            fragmentShader = _myAssetProvider->getStringFromFile(DEFAULT_TEXTURED_FRAGMENT_SHADER); 
        } else {
            AC_ERROR << "unknown material mode";
            return;
        }

        shaderProgram = createProgram(vertexShader.c_str(), fragmentShader.c_str());
        if (!shaderProgram) {
            AC_ERROR << "Could not create program.";
            return;
        }

        mvpHandle = glGetUniformLocation(shaderProgram, "u_mvpMatrix");

        //TODO: material inheritance?
        if (_myMaterialMode == UNLIT_COLORED_MATERIAL) {
            colorHandle = glGetUniformLocation(shaderProgram, "a_color");
        } else if (_myMaterialMode ==  UNLIT_TEXTURED_MATERIAL) {
            textureHandle = glGetUniformLocation(shaderProgram, "s_textureMap");
        } else {
            AC_ERROR << "unknown material mode";
            return;
        }
    }
}

