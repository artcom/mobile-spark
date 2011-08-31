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
        _myPos = theXMLNode->getVector2Value("pos", vector2(0,0));
        _mySize = theXMLNode->getVector2Value("size", vector2(1,1));
        _myGLViewport = ViewportPtr(new Viewport(_mySize[0],_mySize[1], _myPos[0],_myPos[1]));
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
        WidgetPtr myWidget = boost::static_pointer_cast<spark::Widget>(theWorld);
        myWidget->render(matrixStack, _myCamera->getProjectionMatrix()); 
    }
    
    void
    View::activate(float theCanvasWidth, float theCanvasHeight) {
        ensureCamera();
        matrixStack.clear();        
        _myCamera->activate(_mySize[0] *theCanvasWidth ,_mySize[1] *theCanvasHeight);
        _myGLViewport->activate(theCanvasWidth, theCanvasHeight);
    }    
}
