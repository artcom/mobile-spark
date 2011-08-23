#include "ShapeWidget.h"

namespace spark {
    ShapeWidget::ShapeWidget(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent)
        : Widget(theApp, theXMLNode, theParent)
    {}

    ShapeWidget::~ShapeWidget() {}

    void ShapeWidget::render(MatrixStack& theCurrentMatrixStack, matrix theProjectionMatrix) const {
        theCurrentMatrixStack.push();
        theCurrentMatrixStack.multMatrix(_myLocalMatrixStack.getTop());

        //renderShape();
        theCurrentMatrixStack.push();
        theCurrentMatrixStack.multMatrixLocal(theProjectionMatrix);
        _myShape->render(theCurrentMatrixStack.getTop());
        theCurrentMatrixStack.pop();            

        Widget::render(theCurrentMatrixStack, theProjectionMatrix);
        /*for (std::vector<ComponentPtr>::const_iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            (*it)->render(theCurrentMatrixStack, theProjectionMatrix);
        }*/

        theCurrentMatrixStack.pop();
    }
    
}
