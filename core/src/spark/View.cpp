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
        vector2 myPos = theXMLNode->getVector2Value("pos", vector2(0,0));
        vector2 mySize = theXMLNode->getVector2Value("size", vector2(1,1));
        AC_PRINT << "view Pos : " << myPos[0] << " / " << myPos[1];
        AC_PRINT << "view size : " << mySize[0] << " / " << mySize[1];
        _myGLViewport = ViewportPtr(new Viewport(mySize[0],mySize[1], myPos[0],myPos[1]));
        
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
