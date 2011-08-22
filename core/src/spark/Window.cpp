#include "Window.h"

#include <mar/openGL_functions.h>
#include "SparkComponentFactory.h"


namespace spark {

    //needed for component factory
    //namespace  {
        ComponentPtr createWindow(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent) {
            return WindowPtr(new Window(theApp, theXMLNode, theParent));
        };
        //const bool registered = spark::SparkComponentFactory::get().registerComponent("Window", spark::createWindow);
    //}


    Window::Window(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, 
                   ComponentPtr theParent):
        Widget(theApp, theXMLNode, theParent) {

        _myWidth = _myXMLNode->getFloatValue("width",100);
        _myHeight = _myXMLNode->getFloatValue("height",100);
        _myClearColor = _myXMLNode->getFloatArrayValue("clearColor");
        _myClearColor = _myClearColor.size() > 2 ? _myClearColor : std::vector<float>(4,1.0f);
        if (_myClearColor.size() == 3) {
            _myClearColor.push_back(1.0f);
        }

        printGLInfo();

        glEnable(GL_DEPTH_TEST); 
        glDepthMask(GL_TRUE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        //create projection matrix
        matrixStack.push();
        matrixStack.loadOrtho(-10.0, 10.0, -10.5f, 10.5f, -0.1f, 100.0f);

        projectionMatrix = matrixStack.getTop();
        matrixStack.pop();

        //create viewport
        glViewport(0, 0, _myWidth, _myHeight);
        checkGlError("glViewport");
    }

    Window::~Window() {
    }

    void Window::render() const {
        AC_PRINT << "in render()";
        glClearColor(_myClearColor[0],_myClearColor[1],_myClearColor[2],_myClearColor[3]);
        checkGlError("glClearColor");
        glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        checkGlError("glClear");
        Widget::render();
    }

    void Window::renderShape(const ShapePtr theShape) {
        matrixStack.push();
        matrixStack.multMatrixLocal(projectionMatrix);
        theShape->render(matrixStack.getTop());
        matrixStack.pop();
    }
}
