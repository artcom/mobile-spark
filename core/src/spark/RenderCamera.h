#ifndef _included_mobile_spark_RenderCamera_
#define _included_mobile_spark_RenderCamera_

#include <masl/MatrixStack.h>
#include <masl/numeric_functions.h>

#include "Widget.h"


namespace spark {

    enum ProjectionType {
        PERSPECTIVE,
        ORTHONORMAL,
        AUTO_ORTHONORMAL
    };


    class RenderCamera : public Widget {
    public:
        RenderCamera(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~RenderCamera();
        void activate(float theCanvasWidth, float theCanvasHeight);
        const matrix & getProjectionMatrix();

        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return RenderCamera::SPARK_TYPE;};
    private:
        matrix _myProjectionMatrix;
        MatrixStack matrixStack;
        ProjectionType _myProjectionType;
        vector3 _myPerspectiveParams;

        static const char * const PerspectiveStr;
        static const char * const OrtohonormalStr;
        static const char * const AutoOrthonormalStr;
    };

    typedef masl::Ptr<RenderCamera> RenderCameraPtr;
};
#endif
