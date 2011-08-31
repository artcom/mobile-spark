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
        const std::string & getWorldName() { return _myWorldName;}
        void renderWorld(ComponentPtr theWorld);
            
    private:      
        void ensureCamera();
                      
        MatrixStack matrixStack;
        matrix projectionMatrix;
        CameraPtr _myCamera;
        mar::ViewportPtr _myGLViewport;
        std::string _myWorldName;
        vector2 _myPos;
        vector2 _mySize;            
    };

    typedef boost::shared_ptr<View> ViewPtr;
};
#endif
