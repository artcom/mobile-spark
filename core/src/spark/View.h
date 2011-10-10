#ifndef _included_mobile_spark_View_
#define _included_mobile_spark_View_

#include <mar/Viewport.h>
#include <masl/MatrixStack.h>

#include "RenderCamera.h"

namespace masl {
    class XMLNode;
    typedef masl::Ptr<XMLNode> XMLNodePtr;
};

namespace spark {

    class View : public Widget {
    public:
        View(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~View();
        void activate(float theCanvasWidth, float theCanvasHeight);
        const std::string & getWorldName() const { return _myWorldName;}
        void renderWorld(ComponentPtr theWorld);
        RenderCameraPtr getCamera() { ensureCamera(); return _myCamera; };
        mar::ViewportPtr getViewport() const { return _myGLViewport; };

        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return View::SPARK_TYPE;};
    private:
        void ensureCamera();

        MatrixStack matrixStack;
        RenderCameraPtr _myCamera;
        mar::ViewportPtr _myGLViewport;
        std::string _myWorldName;
    };

    typedef masl::Ptr<View> ViewPtr;
};
#endif
