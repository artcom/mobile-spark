#ifndef _included_mobile_spark_Window_
#define _included_mobile_spark_Window_

#include "mar/GlHeaders.h"

#include <masl/MatrixStack.h>
#include <masl/XMLNode.h>
#include <mar/Geometry.h>

#include "Widget.h"

using namespace mar;

namespace spark {
    ComponentPtr createWindow(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
    class Window : public Widget {
    public: 
        Window(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
        virtual ~Window();

        MatrixStack matrixStack;
        matrix projectionMatrix;

        virtual void render() const;
        virtual void renderShape(const ShapePtr theShape);
        virtual void onSizeChanged(int theWidth, int theHeight);
    private:
        int _myWidth;
        int _myHeight;
        bool _myFullScreenFlag;
        std::vector<float> _myClearColor;
    };

    typedef boost::shared_ptr<Window> WindowPtr;
};
#endif
