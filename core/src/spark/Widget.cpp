#include "Widget.h"

#include "BaseApp.h"

namespace spark {
    Widget::Widget(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent)
        : Container(theApp, theXMLNode, theParent), _alpha(1.0), _visible(true), _sensible(true) 
    { 
        _x = _myXMLNode->getAttributeAs<float>("x", 0);
        _y = _myXMLNode->getAttributeAs<float>("y", 0);
        _z = _myXMLNode->getAttributeAs<float>("z", 0);
        _scaleX = _myXMLNode->getAttributeAs<float>("scaleX",1);
        _scaleY = _myXMLNode->getAttributeAs<float>("scaleY",1);
        _scaleZ = _myXMLNode->getAttributeAs<float>("scaleZ",1);
        _rotationX = _myXMLNode->getAttributeAs<float>("rotationX", 0);
        _rotationY = _myXMLNode->getAttributeAs<float>("rotationY", 0);
        _rotationZ = _myXMLNode->getAttributeAs<float>("rotationZ", 0);
        _visible = _myXMLNode->getAttributeAs<bool>("visible", _visible);
        _sensible = _myXMLNode->getAttributeAs<bool>("sensible", _sensible);

        updateMatrix();
    }

    Widget::~Widget() {
    }

    void Widget::updateMatrix() {
        MatrixStack helpMatrixStack;
        helpMatrixStack.loadIdentity();
        helpMatrixStack.rotateXAxis(_rotationX);
        helpMatrixStack.rotateYAxis(_rotationY);
        helpMatrixStack.rotateZAxis(_rotationZ);
        helpMatrixStack.translate(_x, _y, _z);
        helpMatrixStack.scale(_scaleX, _scaleY, _scaleZ);
        _myLocalMatrix = helpMatrixStack.getTop();
    };
    
    //TODO: use visitor? 
    void Widget::prerender(MatrixStack& theCurrentMatrixStack) {
        theCurrentMatrixStack.push();
        theCurrentMatrixStack.multMatrix(_myLocalMatrix);
        _myWorldMVMatrix = theCurrentMatrixStack.getTop();
        for (std::vector<ComponentPtr>::const_iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            (*it)->prerender(theCurrentMatrixStack);
        }
        theCurrentMatrixStack.pop();
    }
    bool Widget::isRendered() const {
        if (!_visible) {
            return false;
        } else {
            if (getParent()) {
                return getParent()->isRendered();
            } else {
                return _visible;
            }
        }
    }

    void Widget::render(const matrix & theProjectionMatrix) const {
    }
}
