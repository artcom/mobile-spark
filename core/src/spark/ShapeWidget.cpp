#include "ShapeWidget.h"

namespace spark {
    ShapeWidget::ShapeWidget(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent)
        : Widget(theApp, theXMLNode, theParent)
    {}

    ShapeWidget::~ShapeWidget() {}

    void ShapeWidget::renderWithLocalMatrix(MatrixStack& theCurrentMatrixStack) const {
        _myShape->render(theCurrentMatrixStack.getTop());                
    }
    
}
