#include "Visitors.h"

namespace spark {

    ComponentVisitor::~ComponentVisitor() {
    }

    bool
    PrintComponentVisitor::visit(ComponentPtr theComponent) {
        AC_PRINT << "component name " << theComponent->getName();
        return true;
    }

    bool
    OnPauseComponentVisitor::visit(ComponentPtr theComponent) {
        AC_DEBUG << theComponent->getName() << " onPause";
        theComponent->onPause();
        return true;
    }

    bool
    RealizeComponentVisitor::visit(ComponentPtr theComponent) {
        AC_DEBUG << theComponent->getName() << " realize";
        theComponent->realize();
        return true;
    }

    bool
    OnResumeComponentVisitor::visit(ComponentPtr theComponent) {
        AC_DEBUG << theComponent->getName() << " onResume";
        theComponent->onResume();
        return true;
    }

    CollectAABBComponentVisitor::CollectAABBComponentVisitor(std::vector<std::pair<ComponentPtr, float> > & theList,
                             const float x, const float y,
                             const matrix theProjectionMatrix) :
                             ComponentVisitor(),
                             list_(theList), x_(x), y_(y),
                             projectionMatrix_(theProjectionMatrix) {
    }

    bool
    CollectAABBComponentVisitor::visit(ComponentPtr theComponent) {
        WidgetPtr myWidget = boost::dynamic_pointer_cast<Widget>(theComponent);
        if (!myWidget) {
            return true; // continue traversal
        } else {
            if (!myWidget->isSensible()) {
                return false;
            } else {
                ShapeWidgetPtr myShapeWidget = boost::dynamic_pointer_cast<ShapeWidget>(theComponent);
                if (myShapeWidget && myShapeWidget->getShape() &&
                    myShapeWidget->AABB2Dcontains(x_,y_,projectionMatrix_)) {
                    list_.push_back(std::make_pair(myShapeWidget, myShapeWidget->getWorldZ()));
                    //AC_PRINT << "collect " << myShapeWidget->getName() << ", " << myShapeWidget->getZ() << "  current size " << list_.size();
                }
                return true;
            }
        }
    }

    CollectVisibleNodesVisitor::CollectVisibleNodesVisitor(
            RenderList & theList) :
                             ComponentVisitor(),
                             list_(theList) {
    }

    bool
    CollectVisibleNodesVisitor::visit(ComponentPtr theComponent) {
        WidgetPtr myWidget = boost::dynamic_pointer_cast<Widget>(theComponent);
        if (!myWidget) {
            return true; // continue traversal
        } else {
            if (!myWidget->isVisible()) {
                return false;
            } else {
                ShapeWidgetPtr myShapeWidget = boost::dynamic_pointer_cast<ShapeWidget>(theComponent);
                if (myShapeWidget && myShapeWidget->getShape()) {
                    RenderKey myKey(myShapeWidget->getWorldZ(), myShapeWidget->getShape()->isTransparent());
                    list_.push_back(std::make_pair(myShapeWidget, myKey));
                    //AC_PRINT << "collect " << myShapeWidget->getName() << ", " << myShapeWidget->getZ() << "  current size " << list_.size();
                }
                return true;
            }
        }
    }

    bool
    sortByRenderKey(std::pair<ComponentPtr, RenderKey> i, std::pair<ComponentPtr, RenderKey> j) {
        if (i.second.transparency_ && j.second.transparency_) {
            return (i.second.z_ < j.second.z_);
        } else {
            return (!i.second.transparency_);
        }
    }

}

