#ifndef _included_mobile_spark_Window_
#define _included_mobile_spark_Window_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "../base/MatrixStack.h"
#include "../base/Geometry.h"
#include "../base/XMLNode.h"

#include "Widget.h"

using namespace asl;

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
