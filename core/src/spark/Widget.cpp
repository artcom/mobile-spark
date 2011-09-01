#include "Widget.h"

#include "BaseApp.h"

namespace spark {
    Widget::Widget(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent)
        : Container(theApp, theXMLNode, theParent), _alpha(1.0), _visible(false) 
    { 
        _x = _myXMLNode->getFloatValue("x");
        _y = _myXMLNode->getFloatValue("y");
        _z = _myXMLNode->getFloatValue("z");
        _scaleX = _myXMLNode->getFloatValue("scaleX",1);
        _scaleY = _myXMLNode->getFloatValue("scaleY",1);
        _scaleZ = _myXMLNode->getFloatValue("scaleZ",1);
        _rotationX = _myXMLNode->getFloatValue("rotationX");
        _rotationY = _myXMLNode->getFloatValue("rotationY");
        _rotationZ = _myXMLNode->getFloatValue("rotationZ");

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
    
    
    void Widget::render(MatrixStack& theCurrentMatrixStack, const matrix & theProjectionMatrix) const {
        theCurrentMatrixStack.push();
        theCurrentMatrixStack.multMatrix(_myLocalMatrix);

        renderWithLocalMatrix(theCurrentMatrixStack.getTop(), theProjectionMatrix);
        
        for (std::vector<ComponentPtr>::const_iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            (*it)->render(theCurrentMatrixStack, theProjectionMatrix);
        }
        theCurrentMatrixStack.pop();
    }
}
