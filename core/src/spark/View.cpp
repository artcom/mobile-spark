#include "View.h"

#include <string>

#include "SparkComponentFactory.h"
#include "Widget.h"
#include <mar/openGL_functions.h>
#include <string>

using namespace mar;
using namespace std;

namespace spark {

    //needed for component factory
    //namespace  {
        ComponentPtr createView(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent) {
            return ViewPtr(new View(theApp, theXMLNode, theParent));
        };
        //const bool registered = spark::SparkComponentFactory::get().registerComponent("Window", spark::createWindow);
    //}


    View::View(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, 
                   ComponentPtr theParent):
        Container(theApp, theXMLNode, theParent){
        _myWorldName  = theXMLNode->getStringValue("world");
        vector2 myPos = theXMLNode->getVector2Value("pos", vector2(0,0));
        vector2 mySize = theXMLNode->getVector2Value("size", vector2(1,1));
        _myGLViewport = ViewportPtr(new Viewport(mySize[0],mySize[1], myPos[0],myPos[1]));
        ensureCamera();
    }

    View::~View() {
    }
            
    void 
    View::ensureCamera() {
        if (_myCamera) {
            return;
        }
        string myCameraName = _myXMLNode->getStringValue("cameraName");        
        // find camera
        _myCamera = boost::static_pointer_cast<spark::Camera>(getRoot()->getChildByName(myCameraName, true));
    }
    
    void
    View::renderWorld(ComponentPtr theWorld) {
        ensureCamera();
        WidgetPtr myWorld = boost::static_pointer_cast<spark::Widget>(theWorld);
        myWorld->prerender(matrixStack);
        myWorld->render(_myCamera->getProjectionMatrix()); 
    }
    
    void
    View::activate(float theCanvasWidth, float theCanvasHeight) {
        ensureCamera();
        matrixStack.clear();        
        vector2 mySize = _myGLViewport->getSize();
        _myCamera->activate(mySize[0] * theCanvasWidth, mySize[1] * theCanvasHeight);
        _myGLViewport->activate(theCanvasWidth, theCanvasHeight);
    }    
}
