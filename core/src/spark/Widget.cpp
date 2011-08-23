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
        _myLocalMatrixStack.loadIdentity();
        _myLocalMatrixStack.rotateXAxis(_rotationX);
        _myLocalMatrixStack.rotateYAxis(_rotationY);
        _myLocalMatrixStack.rotateZAxis(_rotationZ);
        _myLocalMatrixStack.translate(_x, _y, _z);
        _myLocalMatrixStack.scale(_scaleX, _scaleY, _scaleZ);
    };
    
    
    void Widget::render(MatrixStack& theCurrentMatrixStack, matrix theProjectionMatrix) const {
        theCurrentMatrixStack.push();
        theCurrentMatrixStack.multMatrix(_myLocalMatrixStack.getTop());

        theCurrentMatrixStack.push();
        theCurrentMatrixStack.multMatrixLocal(theProjectionMatrix);
        
        renderWithLocalMatrix(theCurrentMatrixStack);
                
        theCurrentMatrixStack.pop();            
        
        for (std::vector<ComponentPtr>::const_iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            (*it)->render(theCurrentMatrixStack, theProjectionMatrix);
        }
        theCurrentMatrixStack.pop();
    }
}
