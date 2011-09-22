#include "Visitors.h"

#include "I18nContext.h"

namespace spark {

    ComponentVisitor::~ComponentVisitor() {
    }

    bool
    PrintComponentVisitor::visit(ComponentPtr theComponent) {
        AC_PRINT << "component name " << *theComponent;
        return true;
    }

    bool
    OnPauseComponentVisitor::visit(ComponentPtr theComponent) {
        AC_DEBUG << *theComponent << " onPause";
        theComponent->onPause();
        return true;
    }

    bool
    RealizeComponentVisitor::visit(ComponentPtr theComponent) {
        AC_DEBUG << *theComponent << " realize";
        theComponent->realize();
        return true;
    }

    bool
    OnResumeComponentVisitor::visit(ComponentPtr theComponent) {
        AC_DEBUG << *theComponent << " onResume";
        theComponent->onResume();
        return true;
    }

    bool
    I18nComponentVisitor::visit(ComponentPtr theComponent) {
        AC_DEBUG << *theComponent << " i18n setup";
        WidgetPtr myWidget = boost::dynamic_pointer_cast<Widget>(theComponent);
        if(myWidget && myWidget->getI18nContext()) {
            myWidget->getI18nContext()->setup();
        }
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
                    AC_TRACE << "CollectAABBComponentVisitor collected: " << *myShapeWidget << ", z: " 
                             << myShapeWidget->getZ() << "  current size " << list_.size();
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
                    AC_TRACE << "CollectVisibleNodesVisitor collected: " << *myShapeWidget << ", worldZ: " 
                             << myShapeWidget->getWorldZ() << " isTransparent: " << myShapeWidget->getShape()->isTransparent() 
                             << "  current size " << list_.size();
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

