#include "Material.h"

#include <masl/CallStack.h>
#include <masl/Logger.h>

#include "AssetProvider.h"
#include "Texture.h"
#include "Element.h"
#include "openGL_functions.h"



namespace mar {

    DEFINE_EXCEPTION(ProblemWithHandleException, masl::Exception)

    Material::Material() : alpha_(1.0),
                           shaderProgram_(0), mvpHandle_(0), alphaHandle_(0) 
    {
        AC_DEBUG << "Material::ctor "<<(void*)this;
    }

    Material::~Material() {
        AC_DEBUG << "Material::dtor "<<(void*)this;
        deleteShader();
    }

    //ANDROID ONLY: gl context is lost, so reset shaders
    void
    Material::resetGL() {
        shaderProgram_ = 0;
    }

    void
    Material::deleteShader() {
        if (shaderProgram_) {
            glUseProgram(0);
            glDeleteProgram(shaderProgram_);
            shaderProgram_ = 0;
            ASSERT_GL("glDeleteProgram", PLUS_FILE_LINE);
        }
    }

    void
    Material::setShader(const std::string & theVertexShader, const std::string & theFragmentShader) {
        AC_DEBUG << "Material::setShader vsh: " << theVertexShader << " fsh: " << theFragmentShader;
        vertexShader_ = AssetProviderSingleton::get().ap()->getStringFromFile(
                          theVertexShader.empty() ? DEFAULT_VERTEX_SHADER : theVertexShader);
    }

    void
    Material::bindAttributes() {
        AC_DEBUG << "Material::bindAttributes "<<(void*)this;
        glBindAttribLocation(shaderProgram_, VERTEX_POS_INDEX, "a_position");
        ASSERT_GL("glBindAttribLocation", PLUS_FILE_LINE);
    }

    void
    Material::initShader() {
        AC_DEBUG << "Material initShader "<<(void*)this;
        if (vertexShader_.empty() || fragmentShader_.empty()) {
            setShader();
        }
        shaderProgram_ = createProgram(vertexShader_, fragmentShader_);
        if (!shaderProgram_) {
            AC_ERROR << "Could not create program.";
            throw ShaderCreationException("problems during shader program creation of " + vertexShader_ + " or/and " + fragmentShader_, PLUS_FILE_LINE);
        }
        bindAttributes();
        setHandles();
        glLinkProgram(shaderProgram_);
        ASSERT_GL("glLinkProgram", PLUS_FILE_LINE);
    }

    void
    Material::unloadShader() {
        //glUseProgram(0); //XXX: is this neccessary?
    }
    void
    Material::loadShader(const matrix & theMatrix) {
        if (!shaderProgram_) {
            initShader();
        }
        glUseProgram(shaderProgram_);
        ASSERT_GL("glUseProgram", PLUS_FILE_LINE);
        glUniformMatrix4fv(mvpHandle_, 1, GL_FALSE, theMatrix.data());
        glUniform1f(alphaHandle_, alpha_);
        for (std::map<std::string, std::pair<GLuint, float> >::const_iterator it = customHandlesAndValues_.begin(); 
                it != customHandlesAndValues_.end(); ++it) {
            AC_TRACE << "set value in shader " << it->first << "  " << it->second.first << "  " << it->second.second;
            glUniform1f(it->second.first, it->second.second);
        }
        ASSERT_GL("Material::loadShader", PLUS_FILE_LINE);
    }

    void
    Material::setHandles() {
        mvpHandle_ = getHandle("u_mvpMatrix");
        alphaHandle_ = getHandle("u_alpha");
        for (std::map<std::string, std::pair<GLuint, float> >::iterator it = customHandlesAndValues_.begin(); 
                it != customHandlesAndValues_.end(); ++it)
        {
            it->second.first = getHandle(it->first);
        }
    }

    void
    Material::setCustomHandles(const std::map<std::string, float> & theCustomHandles) {
        for (std::map<std::string, float>::const_iterator it = theCustomHandles.begin(); it != theCustomHandles.end(); ++it) {
            customHandlesAndValues_[it->first] = std::pair<GLuint, float>(0, it->second);
        }
    }

    GLuint
    Material::getHandle(const std::string & theName) const {
        GLuint myHandle = getShaderVariableHandleUniform(shaderProgram_, theName);
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

    void
    UnlitColoredMaterial::loadShader(const matrix & theMatrix) {
        Material::loadShader(theMatrix);
        glUniform4fv(colorHandle_, 1, &(diffuse_[0]));
        ASSERT_GL("UnlitColoredMaterial::loadShader", PLUS_FILE_LINE);
    }

    void
    UnlitColoredMaterial::setShader(const std::string & theVertexShader, const std::string & theFragmentShader) {
        Material::setShader(theVertexShader, theFragmentShader);
        fragmentShader_ = AssetProviderSingleton::get().ap()->getStringFromFile(
                            theFragmentShader.empty() ? DEFAULT_COLORED_FRAGMENT_SHADER : theFragmentShader);

    }

    void
    UnlitColoredMaterial::setHandles() {
        Material::setHandles();
        colorHandle_ = getHandle("u_color");
    }

    //////////////////////////////////////////////////// UnlitTexturedMaterial
    UnlitTexturedMaterial::UnlitTexturedMaterial(const std::string & theSrc) : Material(), textureMatrixHandle_(0) {
        setTexture(theSrc);
    }

    UnlitTexturedMaterial::~UnlitTexturedMaterial() {
    }

    //ANDROID ONLY: gl context is lost, so reset textures
    void
    UnlitTexturedMaterial::resetGL() {
        Material::resetGL();
        if (texture_) {
            setTexture(texture_->getSrc());
        }
    }

    void
    UnlitTexturedMaterial::setTexture(const TexturePtr theTexture) {
        texture_ = TexturePtr(new Texture());
    }

    void
    UnlitTexturedMaterial::setTexture(const std::string & theSrc) {
        if (theSrc != "") {
            texture_ = TexturePtr(new Texture());
            texture_->setSrc(theSrc);
        }
    }

    void
    UnlitTexturedMaterial::setHandles() {
        Material::setHandles();
        textureMatrixHandle_ = getHandle("u_textureMatrix");
    }

    void
    UnlitTexturedMaterial::loadShader(const matrix & theMatrix) {
        Material::loadShader(theMatrix);
        glUniformMatrix4fv(textureMatrixHandle_, 1, GL_FALSE, texture_->matrix_.data());
        glBindTexture(GL_TEXTURE_2D, texture_->getTextureId());
        ASSERT_GL("UnlitTexturedMaterial::loadShader", PLUS_FILE_LINE);
    }

    void
    UnlitTexturedMaterial::setShader(const std::string & theVertexShader, const std::string & theFragmentShader) {
        Material::setShader(theVertexShader.empty() ? DEFAULT_TEXTURED_VERTEX_SHADER : theVertexShader, theFragmentShader);
        fragmentShader_ = AssetProviderSingleton::get().ap()->getStringFromFile(
                          theFragmentShader.empty() ? DEFAULT_TEXTURED_FRAGMENT_SHADER : theFragmentShader);
    }

    void
    UnlitTexturedMaterial::bindAttributes() {
        Material::bindAttributes();
        glBindAttribLocation(shaderProgram_, VERTEX_TEXCOORD0_INDEX, "a_texCoord0");
        ASSERT_GL("UnlitTexturedMaterial::bindAttributes", PLUS_FILE_LINE);
    }

}

