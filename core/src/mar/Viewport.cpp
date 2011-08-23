#include "Viewport.h"

#include "openGL_functions.h"
#include <masl/Logger.h>


namespace mar {

    Viewport::Viewport() :_myPos(0,0), _mySize(1.0,1.0){
    }
    
    Viewport::Viewport(float theWidth, float theHeight, float theXPos, float theYPos):
        _myPos(theXPos, theYPos), _mySize(theWidth, theHeight) {        
        AC_PRINT << "Viewport setup pos: " << theXPos << " /" << theYPos << " size: " << theWidth << "/" << theHeight;
    }
        

    Viewport::~Viewport() {
    }
    
    void
    Viewport::activate(float theCanvasWidth, float theCanvasHeight) {
        glViewport(_myPos[0], _myPos[1], theCanvasWidth * _mySize[0], theCanvasHeight * _mySize[1]);
        checkGlError("glViewport");        
    }

}


