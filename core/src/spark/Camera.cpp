#include "Camera.h"

#include "SparkComponentFactory.h"

#include <string>

using namespace std;
using namespace mar;

namespace spark {

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
        if (_myProjectionType == ORTHONORMAL) {
            if (_myAutoProjection) {
                AC_PRINT << "############### activate auto aorthonormal frustum";
                _myFrustum = Frustum(0.0f, theCameraWidth , 0.0f, theCameraHeight, -0.1f, 100.0f, ORTHONORMAL);
            } else {
            } 
        } else {
            AC_PRINT << "############### activate projective frustum";
            _myFrustum = Frustum(_myFrustumParams[0], _myFrustumParams[1], _myFrustumParams[2], _myFrustumParams[3], PERSPECTIVE);
        }
        _myFrustum.getProjectionMatrix(_myProjectionMatrix);
        
        
    }    
}
