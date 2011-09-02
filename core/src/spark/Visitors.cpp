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

    CollectAABBComponentVisitor::CollectAABBComponentVisitor(std::vector<std::pair<ComponentPtr, float> > & theList, 
                              const unsigned int x, const unsigned int y) : 
                             ComponentVisitor(),
                             list_(theList), x_(x), y_(y) {
    }

    void 
    CollectAABBComponentVisitor::visit(ComponentPtr theComponent) {
        ShapeWidgetPtr myShapeWidget = boost::dynamic_pointer_cast<ShapeWidget>(theComponent);
        if (myShapeWidget && myShapeWidget->getShape() && myShapeWidget->AABBcontains(x_,y_)) {
            list_.push_back(std::make_pair(myShapeWidget, myShapeWidget->getZ()));
            //AC_PRINT << "collect " << myShapeWidget->getName() << ", " << myShapeWidget->getZ() << "  current size " << list_.size();
        }
    }
}

