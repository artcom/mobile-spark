#include "View.h"

#include "SparkComponentFactory.h"
#include "Widget.h"

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
        _myGLViewport = ViewportPtr(new Viewport(1,1,0,0));
        
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
        //create projection matrix
        matrixStack.push();
        matrixStack.loadOrtho(0.0f, theCanvasWidth, 0.0f, theCanvasHeight, -0.1f, 100.0f);

        projectionMatrix = matrixStack.getTop();
        matrixStack.pop();
          
        _myGLViewport->activate(theCanvasWidth, theCanvasHeight);
    }    
}
