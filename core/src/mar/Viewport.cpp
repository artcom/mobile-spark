// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "Viewport.h"

#include "openGL_functions.h"
#include <masl/Logger.h>


namespace mar {

    Viewport::Viewport() :_myPos(0,0), _mySize(1.0,1.0){
    }

    Viewport::Viewport(float theWidth, float theHeight, float theXPos, float theYPos):
        _myPos(theXPos, theYPos), _mySize(theWidth, theHeight) {
        AC_INFO << "Viewport setup pos: " << theXPos << " /" << theYPos << " size: " << theWidth << "/" << theHeight;
    }


    Viewport::~Viewport() {
    }

    void
    Viewport::activate(float theCanvasWidth, float theCanvasHeight) {
        glViewport(theCanvasWidth * _myPos[0], theCanvasHeight * _myPos[1], theCanvasWidth * _mySize[0], theCanvasHeight * _mySize[1]);
        checkGlError("glViewport");
    }

}


