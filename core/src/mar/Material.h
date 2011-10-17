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
        virtual void createShader(const std::string & theVertexShader = "", const std::string & theFragmentShader = "");
        virtual void loadShader(const matrix & theMatrix);
        virtual void initGL();
        void setAlpha(const float theAlpha) {alpha_ = theAlpha; transparency_ |= (alpha_ != 1.0);};
        void setCustomValues(const std::map<std::string, float> & theCustomValues);
        void setCustomHandles(const std::vector<std::string> & theCustomHandles);

        GLuint shaderProgram;
        GLuint mvpHandle;
        GLuint alphaHandle;
        std::map<std::string, std::pair<GLuint, float> > customHandlesAndValues_; //maps handle string to pair of handle and value
        bool transparency_;

    protected:
        Material();
        virtual void setShader(const std::string & theVertexShader = "", const std::string & theFragmentShader = "");
        virtual void setHandles();
        GLuint getHandle(const std::string & theName) const;

        std::string _myFragmentShader;
        std::string _myVertexShader;
        float alpha_;
    };
    typedef masl::Ptr<Material> MaterialPtr;

    class UnlitColoredMaterial : public Material {
    public:
        UnlitColoredMaterial();
        virtual ~UnlitColoredMaterial();
        virtual void loadShader(const matrix & theMatrix);
        void setDiffuseColor(const vector3 & theColor) {
            diffuse_ = vector4(theColor, alpha_);
            transparency_ |= (diffuse_[3] != 1.0);
        }
        void setDiffuseColor(const vector4 & theColor) {
            diffuse_ = theColor;
            transparency_ |= (diffuse_[3] != 1.0);
        }
        vector4 getDiffuseColor() const {
            return diffuse_;
        }

        vector4 ambient;
        vector4 specular;
        float shininess;
        int illuminationModel;
    private:
        vector4 diffuse_;
        GLuint colorHandle;

    private:
        virtual void setShader(const std::string & theVertexShader = "", const std::string & theFragmentShader = "");
        virtual void setHandles();
    };
    typedef masl::Ptr<UnlitColoredMaterial> UnlitColoredMaterialPtr;

    class UnlitTexturedMaterial : public Material {
    public:
        UnlitTexturedMaterial(const std::string & theSrc, const bool theCacheFlag = false);
        virtual ~UnlitTexturedMaterial();
        virtual void loadShader(const matrix & theMatrix);
        virtual void initGL();
        TexturePtr getTexture() const {return _myTexture;}
        TexturePtr _myTexture;
    private:
        virtual void setShader(const std::string & theVertexShader = "", const std::string & theFragmentShader = "");
        virtual void setHandles();
        std::string _mySrc;
        GLuint textureMatrixHandle;

    };
    typedef masl::Ptr<UnlitTexturedMaterial> UnlitTexturedMaterialPtr;
};

#endif
