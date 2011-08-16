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

    struct Material {
        Material(const AssetProviderPtr theAssetProvider);
        ~Material();

        void createShader();

        std::vector<float> ambient;
        std::vector<float> diffuse;
        std::vector<float> specular;
        float transparency;
        float shininess;
        short illuminationModel;

        std::string textureFile;
        GLuint textureId;
        GLuint shaderProgram;
        GLuint mvpHandle;
        GLuint colorHandle;
        bool rgb;
        const AssetProviderPtr _myAssetProvider;
    };

    typedef boost::shared_ptr<Material> MaterialPtr;
};

#endif 
