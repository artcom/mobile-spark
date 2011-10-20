#ifndef _included_mobile_spark_Camera_
#define _included_mobile_spark_Camera_

#include "ShapeWidget.h"

namespace spark {

    class Camera : public ShapeWidget {
    public:
        Camera(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Camera();
        virtual void prerender(MatrixStack& theCurrentMatrixStack);
        virtual void onPause();

        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Camera::SPARK_TYPE;};
    private:
        void setGeometry();
        bool _myColorConversionFlag;
    };

    typedef masl::Ptr<Camera> CameraPtr;
    typedef MemberFunctionEventCallback<Camera, CameraPtr> CameraCB;

};
#endif
