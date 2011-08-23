#ifndef _ac_mobile_masl_Material_h_included_
#define _ac_mobile_masl_Material_h_included_

#include <map>
#include <vector>
#include <string>
#include "GlHeaders.h"
#include <boost/smart_ptr/shared_ptr.hpp>

#include "AssetProvider.h"

//shader program handles
#define VERTEX_POS_INDEX       0
#define VERTEX_TEXCOORD0_INDEX 1
#define VERTEX_NORMAL_INDEX    2


namespace mar {

    const std::string DEFAULT_VERTEX_SHADER = "assets/shaders/default_vertex.glsl";
    const std::string DEFAULT_FRAGMENT_SHADER = "assets/shaders/default_fragment.glsl";
    const std::string DEFAULT_TEXTURED_FRAGMENT_SHADER = "assets/shaders/default_textured_fragment.glsl";

    //material Modes
    const GLuint UNLIT_COLORED_MATERIAL = 0;
    const GLuint UNLIT_TEXTURED_MATERIAL = 1;

    class Material {
    public:
        Material(const AssetProviderPtr theAssetProvider, const GLuint theMaterialMode = UNLIT_COLORED_MATERIAL);
        ~Material();

        void createShader();

        //material (from obj)
        std::vector<float> ambient;
        std::vector<float> diffuse;
        std::vector<float> specular;
        float transparency;
        float shininess;
        short illuminationModel;

        //texture
        std::string textureFile; //needed?
        GLuint textureId;
        GLuint width;
        GLuint height;

        GLuint shaderProgram;
        GLuint mvpHandle;
        GLuint colorHandle;
        bool rgb;

    private:
        GLuint _myMaterialMode;  //TODO: use inheritance instead?
        const AssetProviderPtr _myAssetProvider;

    };

    typedef boost::shared_ptr<Material> MaterialPtr;
};

#endif 
