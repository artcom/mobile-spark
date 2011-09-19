#ifndef _ac_mobile_masl_Material_h_included_
#define _ac_mobile_masl_Material_h_included_

#include <map>
#include <vector>
#include <string>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <masl/MatrixStack.h>

#include "GlHeaders.h"
#include "Texture.h"

namespace mar {

    const std::string DEFAULT_VERTEX_SHADER = "/default_vertex.glsl";
    const std::string DEFAULT_TEXTURED_VERTEX_SHADER = "/default_textured_vertex.glsl";
    const std::string DEFAULT_COLORED_FRAGMENT_SHADER = "/default_colored_fragment.glsl";
    const std::string DEFAULT_TEXTURED_FRAGMENT_SHADER = "/default_textured_fragment.glsl";

    class Material {
    public:
        virtual ~Material();
        virtual void createShader();
        virtual void loadShader(const matrix & theMatrix);
        virtual void initGL();

        GLuint shaderProgram;
        GLuint mvpHandle;

        bool transparency_;
    protected:
        Material();
        virtual void setShader();
        virtual void setHandles();

        std::string _myFragmentShader;
        std::string _myVertexShader;
    };
    typedef boost::shared_ptr<Material> MaterialPtr;

    class UnlitColoredMaterial : public Material {
    public:
        UnlitColoredMaterial();
        virtual ~UnlitColoredMaterial();
        virtual void loadShader(const matrix & theMatrix);
        void setDiffuseColor(const vector4 & theColor) {
            diffuse_ = theColor;
            transparency_ = (diffuse_[3] != 1.0);
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
        virtual void setShader();
        virtual void setHandles();
    };
    typedef boost::shared_ptr<UnlitColoredMaterial> UnlitColoredMaterialPtr;

    class UnlitTexturedMaterial : public Material {
    public:
        UnlitTexturedMaterial(const std::string & theSrc);
        virtual ~UnlitTexturedMaterial();
        virtual void loadShader(const matrix & theMatrix);
        virtual void initGL();
        TexturePtr getTexture() const {return _myTexture;}
        TexturePtr _myTexture;
    private:
        virtual void setShader();
        std::string _mySrc;

    };
    typedef boost::shared_ptr<UnlitTexturedMaterial> UnlitTexturedMaterialPtr;
};

#endif
