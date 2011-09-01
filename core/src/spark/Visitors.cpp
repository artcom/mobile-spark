#include "Visitors.h"

namespace spark {

    ComponentVisitor::~ComponentVisitor() {
        AC_PRINT << "delete componentVisitor";
    }

    void
    PrintComponentVisitor::visit(ComponentPtr theComponent) {
        AC_PRINT << "component name " << theComponent->getName();
    }

    void 
    OnPauseComponentVisitor::visit(ComponentPtr theComponent) {
        AC_PRINT << theComponent->getName() << " onPause";
    }

    void
    OnResumeComponentVisitor::visit(ComponentPtr theComponent) {
        AC_PRINT << theComponent->getName() << " onResume";
        //special behavior for ShapeWidgets needed
        //XXX: is there a better method than casting like this?
        ShapeWidgetPtr myShapeWidget = boost::dynamic_pointer_cast<ShapeWidget>(theComponent);
        if (myShapeWidget && myShapeWidget->getShape()) {
            myShapeWidget->getShape()->initGL();
        }
    }
}

