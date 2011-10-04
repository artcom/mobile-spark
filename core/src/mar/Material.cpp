#include "Material.h"

#include <masl/CallStack.h>
#include <masl/Logger.h>

#include "AssetProvider.h"
#include "Texture.h"
#include "Element.h"
#include "openGL_functions.h"
#include "png_functions.h"


namespace mar {

    DEFINE_EXCEPTION(ProblemWithHandleException, masl::Exception)

    Material::Material() : transparency_(false), alpha_(1.0) {
    }

    Material::~Material() {
    }

    void Material::createShader(const std::string & theVertexShader, const std::string & theFragmentShader) {
        setShader(theVertexShader, theFragmentShader);
    }
    void Material::initGL() {
        shaderProgram = createProgram(_myVertexShader, _myFragmentShader);
        if (!shaderProgram) {
            AC_ERROR << "Could not create program.";
            throw ShaderCreationException("problems during shader program creation of " + _myVertexShader + " or/and " + _myFragmentShader, PLUS_FILE_LINE);
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

    void Material::setShader(const std::string & theVertexShader, const std::string & theFragmentShader) {
        _myVertexShader = AssetProviderSingleton::get().ap()->getStringFromFile(
                          theVertexShader.empty() ? DEFAULT_VERTEX_SHADER : theVertexShader);
    }

    void Material::setHandles() {
        mvpHandle = getHandle("u_mvpMatrix");
        alphaHandle = getHandle("a_alpha");
    }

    GLuint Material::getHandle(const std::string & theName) const {
        GLuint myHandle = glGetUniformLocation(shaderProgram, theName.c_str());
        if (myHandle > MAX_NUM_HANDLES) {
            AC_ERROR << "Strange Handle " << theName << ". Maybe it is not found in shader.";
            throw ProblemWithHandleException("handle for " + theName + " seems to be strange. Maybe it's not found in shader.", PLUS_FILE_LINE);
        }
        return myHandle;
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

    void UnlitColoredMaterial::setShader(const std::string & theVertexShader, const std::string & theFragmentShader) {
        Material::setShader(theVertexShader, theFragmentShader);
        _myFragmentShader = AssetProviderSingleton::get().ap()->getStringFromFile(
                            theFragmentShader.empty() ? DEFAULT_COLORED_FRAGMENT_SHADER : theFragmentShader);

    }

    void UnlitColoredMaterial::setHandles() {
        Material::setHandles();
        colorHandle = getHandle("a_color");
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

    void UnlitTexturedMaterial::setShader(const std::string & theVertexShader, const std::string & theFragmentShader) {
        _myVertexShader = AssetProviderSingleton::get().ap()->getStringFromFile(
                          theVertexShader.empty() ? DEFAULT_TEXTURED_VERTEX_SHADER : theVertexShader);
        _myFragmentShader = AssetProviderSingleton::get().ap()->getStringFromFile(
                          theFragmentShader.empty() ? DEFAULT_TEXTURED_FRAGMENT_SHADER : theFragmentShader);

    }
    void UnlitTexturedMaterial::initGL() {
        Material::initGL();
        glBindAttribLocation(shaderProgram, VERTEX_TEXCOORD0_INDEX, "a_texCoord0");
    }

}

