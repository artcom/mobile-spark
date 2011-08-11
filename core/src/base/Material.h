#ifndef _ac_mobile_asl_Geometry_h_included_
#define _ac_mobile_asl_Geometry_h_included_

#include <map>
#include <vector>
#include <string>
#include <GLES2/gl2.h>
#include <boost/smart_ptr/shared_ptr.hpp>

//shader program handles
#define VERTEX_POS_INDEX       0
#define VERTEX_TEXCOORD0_INDEX 1
#define VERTEX_NORMAL_INDEX    2


namespace asl {

    const std::string DEFAULT_VERTEX_SHADER = "assets/shaders/default_vertex.glsl";
    const std::string DEFAULT_FRAGMENT_SHADER = "assets/shaders/default_fragment.glsl";

    class BaseApp;
    typedef boost::shared_ptr<BaseApp> BaseAppPtr;

    struct Material {
        Material(const BaseAppPtr theStage);
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
        bool rgb;
        const BaseAppPtr _myStage;
    };

    typedef boost::shared_ptr<Material> MaterialPtr;
};

#endif 
