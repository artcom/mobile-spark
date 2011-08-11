#include "Material.h"
#include "openGL_functions.h"
#include "BaseApp.h"

#ifdef __ANDROID__
#include "../android/APK_functions.h"
#endif

namespace asl {

    Material::Material(const BaseAppPtr theStage) : _myStage(theStage) {

        //TODO: create shader when we have the necessary information
        createShader();
    }

    Material::~Material() {
    }

    void Material::createShader() {
        //TODO: ios???
        std::string vertexShader = android::readFromPackage(_myStage->apkArchive, DEFAULT_VERTEX_SHADER.c_str());
        std::string fragmentShader = android::readFromPackage(_myStage->apkArchive, DEFAULT_FRAGMENT_SHADER.c_str());

        shaderProgram = createProgram(vertexShader.c_str(), fragmentShader.c_str());
        if (!shaderProgram) {
            AC_ERROR << "Could not create program.";
            return;
        }

        mvpHandle = glGetUniformLocation(shaderProgram, "u_mvpMatrix");
    }

}

