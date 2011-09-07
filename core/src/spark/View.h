#ifndef _included_mobile_spark_View_
#define _included_mobile_spark_View_

#include <mar/Viewport.h>
#include <masl/XMLNode.h>
#include <masl/MatrixStack.h>

#include "Container.h"
#include "Camera.h"

namespace spark {
    
    ComponentPtr createView(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
    
    class View : public Container {
    public: 
        View(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
        virtual ~View();
        void activate(float theCanvasWidth, float theCanvasHeight);
        const std::string & getWorldName() const { return _myWorldName;}
        void renderWorld(ComponentPtr theWorld);
        CameraPtr getCamera() const { return _myCamera; };
        mar::ViewportPtr getViewport() const { return _myGLViewport; };
            
    private:      
        void ensureCamera();
                      
        MatrixStack matrixStack;
        CameraPtr _myCamera;
        mar::ViewportPtr _myGLViewport;
        std::string _myWorldName;
    };

    typedef boost::shared_ptr<View> ViewPtr;
};
#endif
