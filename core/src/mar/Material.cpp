#include "Material.h"

#include <masl/Logger.h>

#include "AssetProvider.h"
#include "Element.h"
#include "openGL_functions.h"
#include "png_functions.h"


namespace mar {
    Material::Material() : transparency_(false), alpha_(1.0) {
    }

    Material::~Material() {
    }

    void Material::createShader() {
        setShader();
    }
    void Material::initGL() {
        shaderProgram = createProgram(_myVertexShader, _myFragmentShader);
        if (!shaderProgram) {
            AC_ERROR << "Could not create program.";
            return;
        }
        glBindAttribLocation(shaderProgram, VERTEX_POS_INDEX, "a_position");
        setHandles();
    }

    void Material::loadShader(const matrix & theMatrix) {
        glUseProgram(shaderProgram);
        checkGlError("glUseProgram");
        glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, theMatrix.data());
        glUniform1f(alphaHandle, alpha_);
    }

    void Material::setShader() {
        _myVertexShader = AssetProviderSingleton::get().ap()->getStringFromFile(DEFAULT_VERTEX_SHADER);
    }

    void Material::setHandles() {
        mvpHandle = glGetUniformLocation(shaderProgram, "u_mvpMatrix");
        alphaHandle = glGetUniformLocation(shaderProgram, "a_alpha");
    }

    //////////////////////////////////////////////////// UnlitColoredMaterial
    UnlitColoredMaterial::UnlitColoredMaterial(){
    }

    UnlitColoredMaterial::~UnlitColoredMaterial() {
    }

    void UnlitColoredMaterial::loadShader(const matrix & theMatrix) {
        Material::loadShader(theMatrix);
        glUniform4fv(colorHandle, 1, &(diffuse_[0]));
    }

    void UnlitColoredMaterial::setShader() {
        Material::setShader();
        _myFragmentShader = AssetProviderSingleton::get().ap()->getStringFromFile(DEFAULT_COLORED_FRAGMENT_SHADER);

    }

    void UnlitColoredMaterial::setHandles() {
        Material::setHandles();
        colorHandle = glGetUniformLocation(shaderProgram, "a_color");
    }

    //////////////////////////////////////////////////// UnlitTexturedMaterial
    UnlitTexturedMaterial::UnlitTexturedMaterial(const std::string & theSrc) {
        _mySrc = theSrc;
        _myTexture = TexturePtr(new Texture());

        if (_mySrc != "") {
            loadTextureFromPNG(_mySrc, _myTexture);
            transparency_ = _myTexture->transparency_;
        }
    }

    UnlitTexturedMaterial::~UnlitTexturedMaterial() {
    }

    void UnlitTexturedMaterial::loadShader(const matrix & theMatrix) {
        Material::loadShader(theMatrix);
        glBindTexture(GL_TEXTURE_2D, _myTexture->getTextureId());
    }

    void UnlitTexturedMaterial::setShader() {
        _myVertexShader = AssetProviderSingleton::get().ap()->getStringFromFile(DEFAULT_TEXTURED_VERTEX_SHADER);
        _myFragmentShader = AssetProviderSingleton::get().ap()->getStringFromFile(DEFAULT_TEXTURED_FRAGMENT_SHADER);

    }
    void UnlitTexturedMaterial::initGL() {
        Material::initGL();
        glBindAttribLocation(shaderProgram, VERTEX_TEXCOORD0_INDEX, "a_texCoord0");
    }

}

