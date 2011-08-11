#include "Widget.h"

#include "BaseApp.h"

namespace spark {
    Widget::Widget(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
            _visible(false),
            _alpha(1.0),
            Container(theApp, theXMLNode, theParent) { 
        _x = _myXMLNode->getFloatValue("x");
        _y = _myXMLNode->getFloatValue("y");
        _z = _myXMLNode->getFloatValue("z");
        _scaleX = _myXMLNode->getFloatValue("scaleX",1);
        _scaleY = _myXMLNode->getFloatValue("scaleY",1);
        _scaleZ = _myXMLNode->getFloatValue("scaleZ",1);
        _rotationX = _myXMLNode->getFloatValue("rotationX");
        _rotationY = _myXMLNode->getFloatValue("rotationY");
        _rotationZ = _myXMLNode->getFloatValue("rotationZ");

        _myLocalMatrixStack.loadIdentity();
        _myLocalMatrixStack.scaleLocal(_scaleX, _scaleY, _scaleZ);
        _myLocalMatrixStack.rotateXAxisLocal(_rotationX);
        _myLocalMatrixStack.rotateYAxisLocal(_rotationY);
        _myLocalMatrixStack.rotateZAxisLocal(_rotationZ);
        _myLocalMatrixStack.translateLocal(_x, _y, _z);
    }

    Widget::~Widget() {
    }

    void Widget::render() const {
        _myApp->window->matrixStack.push();
        _myApp->window->matrixStack.multMatrixLocal(_myLocalMatrixStack.getTop());

        drawObject();
        for (std::vector<ComponentPtr>::const_iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            (*it)->render();
        }

        _myApp->window->matrixStack.pop();
    }
}
