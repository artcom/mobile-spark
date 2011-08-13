#ifndef _included_mobile_spark_Window_
#define _included_mobile_spark_Window_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

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
        virtual void renderShape(const ShapePtr theShape);
    private:
        int _myWidth;
        int _myHeight;
        std::vector<float> _myClearColor;
    };

    typedef boost::shared_ptr<Window> WindowPtr;

    //needed for component factory
    ComponentPtr createWindow(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
};
#endif
