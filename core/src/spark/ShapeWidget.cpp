#include "ShapeWidget.h"

namespace spark {
    ShapeWidget::ShapeWidget(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent)
        : Widget(theApp, theXMLNode, theParent)
    {}

    ShapeWidget::~ShapeWidget() {}

    void ShapeWidget::render(MatrixStack& theCurrentMatrixStack, matrix theProjectionMatrix) const {
        theCurrentMatrixStack.push();
        theCurrentMatrixStack.multMatrix(_myLocalMatrixStack.getTop());

        theCurrentMatrixStack.push();
        theCurrentMatrixStack.multMatrixLocal(theProjectionMatrix);
        _myShape->render(theCurrentMatrixStack.getTop());
        theCurrentMatrixStack.pop();            

        Widget::render(theCurrentMatrixStack, theProjectionMatrix);

        theCurrentMatrixStack.pop();
    }
    
}
