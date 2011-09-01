#include "Camera.h"

#include "SparkComponentFactory.h"

#include <string>

using namespace std;
using namespace mar;
using namespace masl;

namespace spark {

    const char* Camera::PerspectiveStr = "perspective";
    const char* Camera::OrtohonormalStr = "orthonormal";
    const char* Camera::AutoOrthonormalStr = "auto";


    //needed for component factory
    //namespace  {
        ComponentPtr createCamera(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent) {
            return CameraPtr(new Camera(theApp, theXMLNode, theParent));
        };
        //const bool registered = spark::SparkComponentFactory::get().registerComponent("Window", spark::createWindow);
    //}


    Camera::Camera(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, 
                   ComponentPtr theParent):
        Widget(theApp, theXMLNode, theParent){     
        bool myFrustumSpecified = false;            
        string myFrustum = theXMLNode->getStringValue("frustum");                           
        size_t myIndex  = myFrustum.find(OrtohonormalStr);
        if (myIndex == 0) {
             size_t myOpening  = myFrustum.find("[", string(OrtohonormalStr).size());
             size_t myClosing  = myFrustum.find("]", string(OrtohonormalStr).size());
             if (myOpening != string::npos && myClosing != string::npos) {
                 string myParams = myFrustum.substr(myOpening+1, myClosing-myOpening-1);
                 if (myParams == AutoOrthonormalStr) {
                    //AC_PRINT << "Create camera with : " << OrtohonormalStr << " : " << myOpening << " / " << myClosing << "->" << myParams;
                    _myProjectionType = ORTHONORMAL;
                    _myAutoProjection = true;
                    myFrustumSpecified = true;
                }
            }
        } else {
            myIndex = myFrustum.find(PerspectiveStr);
            if (myIndex == 0) {
                 size_t myOpening  = myFrustum.find("[", string(OrtohonormalStr).size());
                 size_t myClosing  = myFrustum.find("]", string(OrtohonormalStr).size());
                 if (myOpening != string::npos && myClosing != string::npos) {
                     string myParamsStr = myFrustum.substr(myOpening+1, myClosing-myOpening-1);
                     fromString(myParamsStr, _myFrustumParams);
                     //AC_PRINT << "Create camera with : " << PerspectiveStr << " : " << myOpening << " / " << myClosing << "->" << _myFrustumParams;
                     _myProjectionType = PERSPECTIVE;
                 }
            }
        }
        if (myFrustumSpecified) {
            AC_WARNING << "Create camera with : unknown frustum: " << myFrustum << ", using auto orthonormal";
            _myProjectionType = ORTHONORMAL;
            _myAutoProjection = true;
        }
    }

    Camera::~Camera() {
    }        
    
    const matrix & 
    Camera::getProjectionMatrix() {
        return _myProjectionMatrix;
    }

    void
    Camera::activate(float theCameraWidth, float theCameraHeight) {   
        matrixStack.push();
             
        if (_myProjectionType == ORTHONORMAL) {
            if (_myAutoProjection) {
                matrixStack.loadOrtho(0, theCameraWidth, 0.0 , theCameraHeight, -0.1, 1000);
            } else {
            } 
        } else {
            float myRatio = (float)theCameraWidth/(float)theCameraHeight;
            matrixStack.loadPerspective(-0.1*myRatio, 0.1*myRatio, -0.1*myRatio , 0.1*myRatio, 0.1, 1000);
            matrix myCameraMatrix = _myLocalMatrixStack.getTop();
            myCameraMatrix.inverse();
            matrixStack.multMatrix(myCameraMatrix);
        }
        _myProjectionMatrix = matrixStack.getTop();
        matrixStack.pop();
        
    }    
}
