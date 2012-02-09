// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_View_
#define _included_mobile_spark_View_

#include <masl/MatrixStack.h>

#include "RenderCamera.h"

namespace masl {
    class XMLNode;
    typedef masl::Ptr<XMLNode> XMLNodePtr;
};
namespace mar {
    class Viewport;
    typedef masl::Ptr<Viewport> ViewportPtr;
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
        virtual std::string getAttributesAsString() const;
    private:
        void ensureCamera();

        MatrixStack matrixStack;
        RenderCameraPtr _myCamera;
        mar::ViewportPtr _myGLViewport;
        std::string _myWorldName;
        float _myCanvasWidth;
        float _myCanvasHeight;
    };

    typedef masl::Ptr<View> ViewPtr;
};
#endif
