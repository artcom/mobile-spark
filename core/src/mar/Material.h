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

    const std::string DEFAULT_VERTEX_SHADER = "assets/shaders/default_vertex.glsl";
    const std::string DEFAULT_TEXTURED_VERTEX_SHADER = "assets/shaders/default_textured_vertex.glsl";
    const std::string DEFAULT_COLORED_FRAGMENT_SHADER = "assets/shaders/default_colored_fragment.glsl";
    const std::string DEFAULT_TEXTURED_FRAGMENT_SHADER = "assets/shaders/default_textured_fragment.glsl";
            
    class Material {
    public:
        virtual ~Material();
        virtual void createShader();
        virtual void loadShader(const matrix & theMatrix);
        virtual void initGL();

        GLuint shaderProgram;
        GLuint mvpHandle;

        float transparency;
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

        //material (from obj)
        std::vector<float> ambient;
        std::vector<float> diffuse;
        std::vector<float> specular;
        float shininess;
        short illuminationModel;

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
        TexturePtr getTexture() {return _myTexture;}
        //GLuint textureId;
        TexturePtr _myTexture;
        //GLuint width;
        //GLuint height;
    private:
        virtual void setShader();
        std::string _mySrc;     
                   
    };
    typedef boost::shared_ptr<UnlitTexturedMaterial> UnlitTexturedMaterialPtr;
};

#endif 
