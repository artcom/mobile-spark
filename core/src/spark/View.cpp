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
        myWidget->render(matrixStack, projectionMatrix);//_myCamera->getProjectionMatrix());        
    }
    
    void
    View::activate(float theCanvasWidth, float theCanvasHeight) {
        ensureCamera();
        matrixStack.clear();        
        //_myCamera->activate(_mySize[0] *theCanvasWidth ,_mySize[1] *theCanvasHeight);
AC_PRINT << "start";        
        Frustum myFrustum(90.0f,90.0f, 0.5f,10000.0f, PERSPECTIVE);
        myFrustum.getProjectionMatrix(projectionMatrix);
AC_PRINT << "frustum : " << projectionMatrix;
        
        /*matrixStack.push();                
        matrixStack.loadPerspective(0 , theCanvasWidth, 0.0, theCanvasHeight, 0.1, 10000);
        matrixStack.translate(0.0f,0.0f,100.0f);
        projectionMatrix = matrixStack.getTop();
        matrixStack.pop();        */
        
/*vector3 eye, target, up;
eye.set(0,0,0);      // Set 'eye' to (10,10,10)
target.set(0,0,-1);          // Set 'target' to (0,0,0)
up.cardinal(1);         // Set 'up' to (0,1,0), the cardinal Y axis
//matrix view;
cml::matrix_look_at_RH(projectionMatrix, eye, target, up);        
AC_PRINT << "matrix_look_at_RH : " << projectionMatrix;
  */      
//cml::matrix_perspective(projectionMatrix, -1.0f, 1.0f, -1.0f, 1.0f, 0.1f,10000.0f, cml::right_handed, cml::z_clip_zero);        
cml::matrix_perspective(projectionMatrix, -0.5f, 0.5f, -0.5f, 0.5f, 0.1f,10000.0f, cml::right_handed, cml::z_clip_zero);        
AC_PRINT << "matrix_perspective : " << projectionMatrix;
        
        myFrustum.getProjectionMatrix(projectionMatrix);
        _myGLViewport->activate(theCanvasWidth, theCanvasHeight);
    }    
}
