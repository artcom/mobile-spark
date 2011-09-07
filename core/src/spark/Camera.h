#ifndef _included_mobile_spark_Camera_
#define _included_mobile_spark_Camera_

#include "ShapeWidget.h"

using namespace mar;

namespace spark {
    ComponentPtr createCamera(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
    

    class Camera : public ShapeWidget {
    public: 
        Camera(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~Camera();    
        virtual void prerender(MatrixStack& theCurrentMatrixStack);   
        static const char* SPARK_TYPE;             
    private:
        bool _myCameraReadyFlag;
	bool _myCameraCaptureStartedFlag;
    };

    typedef boost::shared_ptr<Camera> CameraPtr;
};
#endif 
