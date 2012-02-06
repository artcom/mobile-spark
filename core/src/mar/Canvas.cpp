// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "Canvas.h"

#include <masl/Logger.h>
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
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // android and ios deliever premultiplied image data for texturing
                                                     // see here: http://www.renderpaz.com/2010/06/premultiplied-alpha-for-iphone-games.html
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_FRONT);
        glDepthRangef(0, 1);
    }

    void
    Canvas::preRender(vector4 theClearColor) {
        AC_TRACE << "Canvas::prerender clearcolor: "<<theClearColor;
        glClearColor(theClearColor[0],theClearColor[1],theClearColor[2],theClearColor[3]);
        checkGlError("glClearColor");
        glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        checkGlError("glClear");

    }

}


