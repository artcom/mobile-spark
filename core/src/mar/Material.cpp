#include "Material.h"

#include <spark/BaseApp.h>  //XXX: can we remove this?

#include "openGL_functions.h"


namespace mar {

    Material::Material(const spark::BaseAppPtr theApp) : _myApp(theApp) {
        //TODO: create shader when we have the necessary information
        createShader();
    }

    Material::~Material() {
    }

    void Material::createShader() {
        //TODO: ios???
        std::string vertexShader = _myApp->assetProvider->getStringFromFile(DEFAULT_VERTEX_SHADER); 
        std::string fragmentShader = _myApp->assetProvider->getStringFromFile(DEFAULT_FRAGMENT_SHADER); 

        shaderProgram = createProgram(vertexShader.c_str(), fragmentShader.c_str());
        if (!shaderProgram) {
            AC_ERROR << "Could not create program.";
            return;
        }

        mvpHandle = glGetUniformLocation(shaderProgram, "u_mvpMatrix");
        colorHandle = glGetUniformLocation(shaderProgram, "a_color");
    }

}

