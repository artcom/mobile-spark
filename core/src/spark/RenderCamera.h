#ifndef _included_mobile_spark_RenderCamera_
#define _included_mobile_spark_RenderCamera_

/*#include <mar/Viewport.h>
#include <masl/XMLNode.h>
#include <masl/Frustum.h>
#include <masl/MatrixStack.h>*/

#include <masl/MatrixStack.h>
#include <masl/numeric_functions.h>

#include "Widget.h"


namespace spark {
    
    enum ProjectionType {
        PERSPECTIVE,
        ORTHONORMAL,
        AUTO_ORTHONORMAL
    };


    ComponentPtr createRenderCamera(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
    
    class RenderCamera : public Widget {
    public: 
        RenderCamera(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
        virtual ~RenderCamera();
        void activate(float theCanvasWidth, float theCanvasHeight);
        const matrix & getProjectionMatrix();            

        static const char* SPARK_TYPE;
    private:                
        matrix _myProjectionMatrix;
        MatrixStack matrixStack;
        ProjectionType _myProjectionType;
        vector3 _myPerspectiveParams;        

        static const char* PerspectiveStr;
        static const char* OrtohonormalStr;
        static const char* AutoOrthonormalStr;
    };

    typedef boost::shared_ptr<RenderCamera> RenderCameraPtr;
};
#endif