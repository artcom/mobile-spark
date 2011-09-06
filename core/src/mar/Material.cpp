#include "Material.h"

#include <masl/Logger.h>

#include "AssetProvider.h"
#include "Element.h"
#include "openGL_functions.h"
#include "png_functions.h"


namespace mar {
    Material::Material() {
    }
    
    Material::~Material() {
    }

    void Material::createShader() {
        setShader();
        initGL();
    }
    void Material::initGL() {
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
        glBindAttribLocation(shaderProgram, VERTEX_POS_INDEX, "a_position");
    }

    void Material::setShader() {
        _myVertexShader = AssetProviderSingleton::get().ap()->getStringFromFile(DEFAULT_VERTEX_SHADER); 
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
        _myFragmentShader = AssetProviderSingleton::get().ap()->getStringFromFile(DEFAULT_COLORED_FRAGMENT_SHADER); 
            
    }

    void UnlitColoredMaterial::setHandles() {
        Material::setHandles();
        colorHandle = glGetUniformLocation(shaderProgram, "a_color");
    }


    //////////////////////////////////////////////////// Texture
    Texture::Texture() :_myTextureId(0){
    }
    
    GLuint Texture::getTextureId() {
        return _myTextureId;
    }
    void Texture::setTextureId(GLuint theTextureId){
        _myTextureId = theTextureId;
    }

    //////////////////////////////////////////////////// UnlitTexturedMaterial
    UnlitTexturedMaterial::UnlitTexturedMaterial(const std::string & theSrc) {
        _mySrc = theSrc;
        _myTexture = TexturePtr(new Texture());
    }

    UnlitTexturedMaterial::~UnlitTexturedMaterial() {
    }

    void UnlitTexturedMaterial::loadShader(const matrix & theMatrix) {
        Material::loadShader(theMatrix);
        glBindTexture(GL_TEXTURE_2D, _myTexture->getTextureId());
        glBindAttribLocation(shaderProgram, VERTEX_TEXCOORD0_INDEX, "a_texCoord0");
    }

    void UnlitTexturedMaterial::setShader() {
        Material::setShader();
        _myFragmentShader = AssetProviderSingleton::get().ap()->getStringFromFile(DEFAULT_TEXTURED_FRAGMENT_SHADER); 
            
    }
    void UnlitTexturedMaterial::initGL() {
        Material::initGL();
        if (_mySrc != "") {
            loadTextureFromPNG(_mySrc, _myTexture);  
            transparency = _myTexture->transparency;
        }
    }
    
}

