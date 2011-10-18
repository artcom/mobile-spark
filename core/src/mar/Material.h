#ifndef _ac_mobile_masl_Material_h_included_
#define _ac_mobile_masl_Material_h_included_

#include <map>
#include <vector>
#include <string>

#include <masl/Exception.h>
#include <masl/Ptr.h>
#include <masl/MatrixStack.h>

#include "GlHeaders.h"
#include "Texture.h"

namespace mar {
    const std::string DEFAULT_VERTEX_SHADER = "/default_vertex.glsl";
    const std::string DEFAULT_TEXTURED_VERTEX_SHADER = "/default_textured_vertex.glsl";
    const std::string DEFAULT_COLORED_FRAGMENT_SHADER = "/default_colored_fragment.glsl";
    const std::string DEFAULT_TEXTURED_FRAGMENT_SHADER = "/default_textured_fragment.glsl";
    const unsigned int MAX_NUM_HANDLES = 23;

    DECLARE_EXCEPTION(ProblemWithHandleException, masl::Exception)

    class Material {
    public:
        virtual ~Material();
        virtual void resetGL();
        virtual void setShader(const std::string & theVertexShader = "", const std::string & theFragmentShader = "");
        virtual void loadShader(const matrix & theMatrix);
        virtual void unloadShader();
        void setAlpha(const float theAlpha) {alpha_ = theAlpha;};
        void setCustomValues(const std::map<std::string, float> & theCustomValues);
        void setCustomHandles(const std::map<std::string, float> & theCustomHandles);

        virtual bool isTransparent() const {return alpha_ != 1.0;};

    protected:
        Material();
        void initShader();
        virtual void setHandles();
        virtual void bindAttributes();
        GLuint getHandle(const std::string & theName) const;

        float alpha_;
        std::string fragmentShader_;
        std::string vertexShader_;
        GLuint shaderProgram_;
        GLuint mvpHandle_;
        GLuint alphaHandle_;
        std::map<std::string, std::pair<GLuint, float> > customHandlesAndValues_; //maps handle string to pair of handle and value
    private:
        void deleteShader();
    };
    typedef masl::Ptr<Material> MaterialPtr;

    class UnlitColoredMaterial : public Material {
    public:
        UnlitColoredMaterial();
        virtual ~UnlitColoredMaterial();
        virtual void setShader(const std::string & theVertexShader = "", const std::string & theFragmentShader = "");
        virtual void loadShader(const matrix & theMatrix);
        void setDiffuseColor(const vector3 & theColor) {
            diffuse_ = vector4(theColor, alpha_);
        }
        void setDiffuseColor(const vector4 & theColor) {
            diffuse_ = theColor;
        }
        vector4 getDiffuseColor() const {
            return diffuse_;
        }
    
        virtual bool isTransparent() const {return Material::isTransparent() || diffuse_[3] != 1.0;};

        //XXX: mixed property visibility
        //are these 4 actually supported?
        vector4 ambient_;
        vector4 specular_;
        float shininess_;
        int illuminationModel_;
    private:
        vector4 diffuse_;
        GLuint colorHandle_;

    private:
        virtual void setHandles();
    };
    typedef masl::Ptr<UnlitColoredMaterial> UnlitColoredMaterialPtr;

    class UnlitTexturedMaterial : public Material {
    public:
        UnlitTexturedMaterial(const std::string & theSrc = "");
        virtual ~UnlitTexturedMaterial();
        virtual void resetGL();
        virtual void setShader(const std::string & theVertexShader = "", const std::string & theFragmentShader = "");
        virtual void loadShader(const matrix & theMatrix);
        TexturePtr getTexture() const {return texture_;}
        void setTexture(const TexturePtr theTexture);
        virtual bool isTransparent() const {return Material::isTransparent() || ((texture_) ? texture_->getTextureInfo()->transparency_ : false);};
    private:
        void setTexture(const std::string & theSrc);
        virtual void setHandles();
        virtual void bindAttributes();
        TexturePtr texture_;
        GLuint textureMatrixHandle_;

    };
    typedef masl::Ptr<UnlitTexturedMaterial> UnlitTexturedMaterialPtr;
};

#endif
