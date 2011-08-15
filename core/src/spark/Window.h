#ifndef _included_mobile_spark_Window_
#define _included_mobile_spark_Window_

#include "mar/GlHeaders.h"

#include <masl/MatrixStack.h>
#include <masl/XMLNode.h>
#include <mar/Geometry.h>

#include "Widget.h"

using namespace mar;

namespace spark {

    class Window : public Widget {
    public: 
        Window(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
        virtual ~Window();

        MatrixStack matrixStack;
        matrix projectionMatrix;

        virtual void render() const;
        virtual void drawObject(const ShapePtr theShape);
    private:
        int _myWidth;
        int _myHeight;
        std::vector<float> _myClearColor;
    };

    typedef boost::shared_ptr<Window> WindowPtr;
};
#endif
