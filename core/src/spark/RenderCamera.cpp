#include "RenderCamera.h"

#include <masl/numeric_functions.h>

#include "SparkComponentFactory.h"

#include <string>

using namespace std;
using namespace mar;
using namespace masl;

namespace spark {

    const char * const RenderCamera::SPARK_TYPE = "RenderCamera";
    const char * const RenderCamera::PerspectiveStr = "perspective";
    const char * const RenderCamera::OrtohonormalStr = "orthonormal";
    const char * const RenderCamera::AutoOrthonormalStr = "auto";
    
    RenderCamera::RenderCamera(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, 
                   ComponentPtr theParent):
        Widget(theApp, theXMLNode, theParent){     
        bool myFrustumSpecified = false;            
        string myFrustum = theXMLNode->getAttributeAs<std::string>("frustum", "");
        size_t myIndex  = myFrustum.find(OrtohonormalStr);
        if (myIndex == 0) {
             size_t myOpening  = myFrustum.find("[", string(OrtohonormalStr).size());
             size_t myClosing  = myFrustum.find("]", string(OrtohonormalStr).size());
             if (myOpening != string::npos && myClosing != string::npos) {
                 string myParams = myFrustum.substr(myOpening, myClosing-myOpening);
                 if (myParams == AutoOrthonormalStr) {
                    _myProjectionType = AUTO_ORTHONORMAL;
                    myFrustumSpecified = true;
                }
            }
        } else {
            myIndex = myFrustum.find(PerspectiveStr);
            if (myIndex == 0) {
                 size_t myOpening  = myFrustum.find("[", string(OrtohonormalStr).size());
                 size_t myClosing  = myFrustum.find("]", string(OrtohonormalStr).size());
                 if (myOpening != string::npos && myClosing != string::npos) {
                     string myParamsStr = myFrustum.substr(myOpening, myClosing-myOpening);
                     fromString(myParamsStr, _myPerspectiveParams);
                     _myProjectionType = PERSPECTIVE;
                     myFrustumSpecified = true;
                 }
            }
        }
        if (!myFrustumSpecified) {
            AC_WARNING << "Create RenderCamera with : unknown frustum: " << myFrustum << ", using auto orthonormal";
            _myProjectionType = AUTO_ORTHONORMAL;
        }
    }

    RenderCamera::~RenderCamera() {
    }        
    
    const matrix & 
    RenderCamera::getProjectionMatrix() {
        return _myProjectionMatrix;
    }

    void
    RenderCamera::activate(float theRenderCameraWidth, float theRenderCameraHeight) {   
        matrixStack.push();
             
        if (_myProjectionType == AUTO_ORTHONORMAL) {
            matrixStack.loadOrtho(0, theRenderCameraWidth, 0.0 , theRenderCameraHeight, -0.1, 1000);
        } else if (_myProjectionType == PERSPECTIVE) {
            float myWing = _myPerspectiveParams[1] * tanf(cml::rad(_myPerspectiveParams[0]) / 2.0);            
            float myRatio = (float)theRenderCameraWidth/(float)theRenderCameraHeight;
            matrixStack.loadPerspective(-myWing, myWing, -myWing / myRatio, myWing / myRatio, _myPerspectiveParams[1], _myPerspectiveParams[2]);
        }
        matrix myRenderCameraMatrix = _myLocalMatrix;
        myRenderCameraMatrix.inverse();
        matrixStack.multMatrix(myRenderCameraMatrix);
        _myProjectionMatrix = matrixStack.getTop();
        matrixStack.pop();
    }    
}
