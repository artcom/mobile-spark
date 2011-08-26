#include "View.h"

#include "SparkComponentFactory.h"
#include "Widget.h"
#include <mar/openGL_functions.h>
using namespace mar;

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
        
    }

    View::~View() {
    }        
    
    void
    View::renderWorld(ComponentPtr theWorld) {
        WidgetPtr myWidget = boost::static_pointer_cast<spark::Widget>(theWorld);
        myWidget->render(matrixStack, projectionMatrix);        
    }
    
    void
    View::activate(float theCanvasWidth, float theCanvasHeight) {
        matrixStack.clear();
        
        matrixStack.push();
        matrixStack.loadOrtho(0.0f, _mySize[0] *theCanvasWidth , 0.0f, _mySize[1] *theCanvasHeight, -0.1f, 100.0f);
        projectionMatrix = matrixStack.getTop();
        matrixStack.pop();          
        _myGLViewport->activate(theCanvasWidth, theCanvasHeight);
    }    
}
