#include "Material.h"

#include <masl/Logger.h>

#include "AssetProvider.h"
#include "openGL_functions.h"


namespace mar {
    Material::Material() {
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

    void Material::loadShader(const matrix & theMatrix) {
        glLinkProgram(shaderProgram);
        glUseProgram(shaderProgram);
        checkGlError("glUseProgram");
        glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, theMatrix.data());
        glBindAttribLocation(shaderProgram, 0, "a_position");
    }

    void Material::setShader() {
        AC_PRINT << "about to get assetprovider";
        _myVertexShader = AssetProviderSingleton::get().ap()->getStringFromFile(DEFAULT_VERTEX_SHADER); 
        AC_PRINT << "got assetprovider";
    }

    void Material::setHandles() {
        mvpHandle = glGetUniformLocation(shaderProgram, "u_mvpMatrix");
    }

    //////////////////////////////////////////////////// UnlitColoredMaterial
    UnlitColoredMaterial::UnlitColoredMaterial(){
    }

    UnlitColoredMaterial::~UnlitColoredMaterial() {
    }

    void UnlitColoredMaterial::loadShader(const matrix & theMatrix) {
        Material::loadShader(theMatrix);
        glUniform4fv(colorHandle, 1, &(diffuse[0]));
    }

    void UnlitColoredMaterial::setShader() {
        Material::setShader();
        AC_PRINT << "about to get assetprovider";
        _myFragmentShader = AssetProviderSingleton::get().ap()->getStringFromFile(DEFAULT_COLORED_FRAGMENT_SHADER); 
        AC_PRINT << "got assetprovider";
            
    }

    void UnlitColoredMaterial::setHandles() {
        Material::setHandles();
        colorHandle = glGetUniformLocation(shaderProgram, "a_color");
    }

    //////////////////////////////////////////////////// UnlitTexturedMaterial
    UnlitTexturedMaterial::UnlitTexturedMaterial() {
    }

    UnlitTexturedMaterial::~UnlitTexturedMaterial() {
    }

    void UnlitTexturedMaterial::loadShader(const matrix & theMatrix) {
        Material::loadShader(theMatrix);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glBindAttribLocation(shaderProgram, 1, "a_texCoord0");
    }

    void UnlitTexturedMaterial::setShader() {
        Material::setShader();
        AC_PRINT << "about to get assetprovider";
        _myFragmentShader = AssetProviderSingleton::get().ap()->getStringFromFile(DEFAULT_TEXTURED_FRAGMENT_SHADER); 
        AC_PRINT << "got assetprovider";
            
    }
}

