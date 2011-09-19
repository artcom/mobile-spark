#ifndef _included_mobile_spark_Camera_
#define _included_mobile_spark_Camera_

#include "ShapeWidget.h"

using namespace mar;

namespace spark {

    class Camera : public ShapeWidget {
    public:
        Camera(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~Camera();
        virtual void prerender(MatrixStack& theCurrentMatrixStack);
        virtual void onPause();
        virtual void onSizeChanged(EventPtr theEvent);

        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Camera::SPARK_TYPE;};
    private:
        void setGeometry();
        bool _myColorConversionFlag;
        bool _mySetupFlag;
        bool _myPortraitMode;
    };

    typedef boost::shared_ptr<Camera> CameraPtr;
    typedef MemberFunctionEventCallback<Camera, CameraPtr> CameraCB;

};
#endif
