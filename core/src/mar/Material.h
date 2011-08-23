#ifndef _ac_mobile_masl_Material_h_included_
#define _ac_mobile_masl_Material_h_included_

#include <map>
#include <vector>
#include <string>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <masl/MatrixStack.h>

#include "AssetProvider.h"
#include "GlHeaders.h"

//shader program handles
#define VERTEX_POS_INDEX       0
#define VERTEX_TEXCOORD0_INDEX 1
#define VERTEX_NORMAL_INDEX    2


namespace mar {

    const std::string DEFAULT_VERTEX_SHADER = "assets/shaders/default_vertex.glsl";
    const std::string DEFAULT_COLORED_FRAGMENT_SHADER = "assets/shaders/default_colored_fragment.glsl";
    const std::string DEFAULT_TEXTURED_FRAGMENT_SHADER = "assets/shaders/default_textured_fragment.glsl";

    class Material {
    public:
        virtual ~Material();
        virtual void createShader();
        virtual void loadShader(const matrix & theMatrix);

        GLuint shaderProgram;
        GLuint mvpHandle;

    protected:
        Material(const AssetProviderPtr theAssetProvider); 
        virtual void setShader();
        virtual void setHandles();

        const AssetProviderPtr _myAssetProvider;
        std::string _myFragmentShader;
        std::string _myVertexShader;
    };
    typedef boost::shared_ptr<Material> MaterialPtr;

    class UnlitColoredMaterial : public Material {
    public:
        UnlitColoredMaterial(const AssetProviderPtr theAssetProvider);
        virtual ~UnlitColoredMaterial();
        virtual void loadShader(const matrix & theMatrix);

        //material (from obj)
        std::vector<float> ambient;
        std::vector<float> diffuse;
        std::vector<float> specular;
        float transparency;
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
        UnlitTexturedMaterial(const AssetProviderPtr theAssetProvider);
        virtual ~UnlitTexturedMaterial();
        virtual void loadShader(const matrix & theMatrix);

        std::string textureFile; //needed?
        GLuint textureId;
        GLuint width;
        GLuint height;
        bool rgb;  //image mode

    private:
        virtual void setShader();
    };
    typedef boost::shared_ptr<UnlitTexturedMaterial> UnlitTexturedMaterialPtr;
};

#endif 
