#ifndef _included_mobile_spark_Camera_
#define _included_mobile_spark_Camera_

/*#include <mar/Viewport.h>
#include <masl/XMLNode.h>
#include <masl/Frustum.h>
#include <masl/MatrixStack.h>*/

#include <masl/MatrixStack.h>
#include <masl/numeric_functions.h>

#include "Widget.h"

enum ProjectionType {
    PERSPECTIVE,
    ORTHONORMAL
};

static const char* PerspectiveStr = "perspective";
static const char* OrtohonormalStr = "orthonormal";
static const char* AutoOrthonormalStr = "auto";

namespace spark {
    
    ComponentPtr createCamera(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
    
    class Camera : public Widget {
    public: 
        Camera(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
        virtual ~Camera();
        void activate(float theCanvasWidth, float theCanvasHeight);
        const matrix & getProjectionMatrix();            
    private:                
        matrix _myProjectionMatrix;
        MatrixStack matrixStack;
        ProjectionType _myProjectionType;
        bool _myAutoProjection;
        vector4 _myFrustumParams;        
    };

    typedef boost::shared_ptr<Camera> CameraPtr;
};
#endif
