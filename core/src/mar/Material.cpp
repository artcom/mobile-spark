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

    Material::Material() : transparency_(false), alpha_(1.0),
                           shaderProgram_(0), mvpHandle_(0), alphaHandle_(0) 
    {}

    Material::~Material() {
    }

    void
    Material::createShader(const std::string & theVertexShader, const std::string & theFragmentShader) {
        setShader(theVertexShader, theFragmentShader);
    }

    void
    Material::bindAttributes() {
        glBindAttribLocation(shaderProgram_, VERTEX_POS_INDEX, "a_position");
    }

    void
    Material::initGL() {
        AC_PRINT << "Material::initGL ";
        if (_myVertexShader.empty() || _myFragmentShader.empty()) {
            setShader();
        }
        shaderProgram_ = createProgram(_myVertexShader, _myFragmentShader);
        if (!shaderProgram_) {
            AC_ERROR << "Could not create program.";
            throw ShaderCreationException("problems during shader program creation of " + _myVertexShader + " or/and " + _myFragmentShader, PLUS_FILE_LINE);
            return;
        }
        bindAttributes();
        setHandles();
        glLinkProgram(shaderProgram_);
    }

    void
    Material::loadShader(const matrix & theMatrix) {
        if (_myVertexShader.empty() || _myFragmentShader.empty()) {
            setShader();
        }
        if (!shaderProgram_) {
            initGL();
        }
        glUseProgram(shaderProgram_);
        checkGlError("glUseProgram");
        glUniformMatrix4fv(mvpHandle_, 1, GL_FALSE, theMatrix.data());
        glUniform1f(alphaHandle_, alpha_);
        for (std::map<std::string, std::pair<GLuint, float> >::const_iterator it = customHandlesAndValues_.begin(); 
                it != customHandlesAndValues_.end(); ++it) {
            AC_TRACE << "set value in shader " << it->first << "  " << it->second.first << "  " << it->second.second;
            glUniform1f(it->second.first, it->second.second);
        }
    }

    void
    Material::setShader(const std::string & theVertexShader, const std::string & theFragmentShader) {
        _myVertexShader = AssetProviderSingleton::get().ap()->getStringFromFile(
                          theVertexShader.empty() ? DEFAULT_VERTEX_SHADER : theVertexShader);
    }

    void
    Material::setHandles() {
        mvpHandle_ = getHandle("u_mvpMatrix");
        alphaHandle_ = getHandle("a_alpha");
    }

    void
    Material::setCustomHandles(const std::map<std::string, float> & theCustomHandles) {
        for (std::map<std::string, float>::const_iterator it = theCustomHandles.begin(); it != theCustomHandles.end(); ++it) {
            customHandlesAndValues_[it->first] = std::pair<GLuint, float>(getHandle(it->first), it->second);
        }
    }

    GLuint
    Material::getHandle(const std::string & theName) const {
        GLuint myHandle = glGetUniformLocation(shaderProgram_, theName.c_str());
        if (myHandle > MAX_NUM_HANDLES) {
            AC_ERROR << "Strange Handle " << theName << ". Maybe it is not found in shader.";
            throw ProblemWithHandleException("handle for " + theName + " seems to be strange. Maybe it's not found in shader.", PLUS_FILE_LINE);
        }
        return myHandle;
    }
    
    void 
    Material::setCustomValues(const std::map<std::string, float> & theCustomValues) {
        for (std::map<std::string, float>::const_iterator it = theCustomValues.begin(); it != theCustomValues.end(); ++it) {
            AC_TRACE << "setCustomValues, search " << it->first;
            std::map<std::string, std::pair<GLuint, float> >::iterator value_it = customHandlesAndValues_.find(it->first);
            if (value_it != customHandlesAndValues_.end()) {
                AC_TRACE << "found -> value " << it->second;
                value_it->second.second = it->second;
            }
        }
    }

    //////////////////////////////////////////////////// UnlitColoredMaterial
    UnlitColoredMaterial::UnlitColoredMaterial() : Material(), diffuse_(1,1,1,1), colorHandle_(0) {
    }

    UnlitColoredMaterial::~UnlitColoredMaterial() {
    }

    void UnlitColoredMaterial::loadShader(const matrix & theMatrix) {
        Material::loadShader(theMatrix);
        glUniform4fv(colorHandle_, 1, &(diffuse_[0]));
    }

    void UnlitColoredMaterial::setShader(const std::string & theVertexShader, const std::string & theFragmentShader) {
        Material::setShader(theVertexShader, theFragmentShader);
        _myFragmentShader = AssetProviderSingleton::get().ap()->getStringFromFile(
                            theFragmentShader.empty() ? DEFAULT_COLORED_FRAGMENT_SHADER : theFragmentShader);

    }

    void UnlitColoredMaterial::setHandles() {
        Material::setHandles();
        colorHandle_ = getHandle("a_color");
    }

    //////////////////////////////////////////////////// UnlitTexturedMaterial
    UnlitTexturedMaterial::UnlitTexturedMaterial(const std::string & theSrc) : Material(), _mySrc(theSrc), textureMatrixHandle_(0) {
        _myTexture = TexturePtr(new Texture());
        if (_mySrc != "") {
            loadTextureFromPNG(_mySrc, _myTexture);
            transparency_ = _myTexture->transparency_;
        }
    }

    UnlitTexturedMaterial::~UnlitTexturedMaterial() {
    }

    void UnlitTexturedMaterial::setHandles() {
        Material::setHandles();
        textureMatrixHandle_ = getHandle("u_textureMatrix");
    }
    void UnlitTexturedMaterial::loadShader(const matrix & theMatrix) {
        Material::loadShader(theMatrix);
        glUniformMatrix4fv(textureMatrixHandle_, 1, GL_FALSE, _myTexture->matrix_.data());
        glBindTexture(GL_TEXTURE_2D, _myTexture->getTextureId());
    }

    void UnlitTexturedMaterial::setShader(const std::string & theVertexShader, const std::string & theFragmentShader) {
        _myVertexShader = AssetProviderSingleton::get().ap()->getStringFromFile(
                          theVertexShader.empty() ? DEFAULT_TEXTURED_VERTEX_SHADER : theVertexShader);
        _myFragmentShader = AssetProviderSingleton::get().ap()->getStringFromFile(
                          theFragmentShader.empty() ? DEFAULT_TEXTURED_FRAGMENT_SHADER : theFragmentShader);

    }

    void
    UnlitTexturedMaterial::bindAttributes() {
        Material::bindAttributes();
        glBindAttribLocation(shaderProgram_, VERTEX_TEXCOORD0_INDEX, "a_texCoord0");
    }

}

