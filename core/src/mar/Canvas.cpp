#include "Canvas.h"

#include "openGL_functions.h"


namespace mar {

    Canvas::Canvas(){

    }

    Canvas::~Canvas() {
    }

    void
    Canvas::initGLState() {
        printGLInfo();

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_FRONT);
        glDepthRangef(0, 1);
    }

    void
    Canvas::preRender(vector4 theClearColor) {
        glClearColor(theClearColor[0],theClearColor[1],theClearColor[2],theClearColor[3]);
        checkGlError("glClearColor");
        glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        checkGlError("glClear");

    }

}


