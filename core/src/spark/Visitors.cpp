// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "Visitors.h"

#include <mar/Shape.h>
#include "World.h"
#include "Window.h"
#include <vector>

using namespace std;

#define DB(x) //x

namespace spark {

    ComponentVisitor::~ComponentVisitor() {
    }

    bool
    PrintComponentVisitor::visit(ComponentPtr theComponent) {
        AC_PRINT << "component name " << *theComponent;
        return true;
    }

    PrintNodeVisitor::PrintNodeVisitor() : depth_(0) {
    }

    bool 
    PrintNodeVisitor::preCheck(ComponentPtr theComponent) {
        depth_++;
        AC_PRINT << std::string( (depth_-1) * 4, ' ' ) << "<" << theComponent->getType() << theComponent->getAttributesAsString() << ">";
        return true;
    }

    bool 
    PrintNodeVisitor::visit(ComponentPtr theComponent) {
        AC_PRINT << std::string( (depth_-1) * 4, ' ' ) << "</" << theComponent->getType() << ">";
        depth_--;
        return true;
    }

    bool
    OnPauseComponentVisitor::visit(ComponentPtr theComponent) {
        AC_DEBUG << *theComponent << " onPause";
        theComponent->onPause();
        return true;
    }

    bool
    ReparentComponentVisitor::visit(ComponentPtr theComponent) {
        AC_DEBUG << *theComponent << " reparent";
        theComponent->reparent();
        return true;
    }

    bool
    RealizeComponentsButWorldAndWindowVisitor::visit(ComponentPtr theComponent) {
        if (theComponent->getType() == World::SPARK_TYPE) {
            return false;
        }
        if (theComponent->getType() == Window::SPARK_TYPE) {
            return true;
        }
        AC_DEBUG << *theComponent << " realize";
        theComponent->realize();
        return true;
    }

    bool
    RealizeComponentsButWorldAndWindowVisitor::preCheck(ComponentPtr theComponent) {
        if (theComponent->getType() == World::SPARK_TYPE) {
            return false;
        }
        return true;
    }
    
    bool
    OnResumeComponentVisitor::visit(ComponentPtr theComponent) {
        AC_DEBUG << *theComponent << " onResume";
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
                    DB(AC_TRACE << "CollectAABBComponentVisitor collected: " << *myShapeWidget << ", z: "
                             << myShapeWidget->getZ() << "  current size " << list_.size();)
                }
                return true;
            }
        }
    }

    CollectVisibleNodesVisitor::CollectVisibleNodesVisitor(
            RenderList & theList, mar::BoundingBox & theScreenBB, const matrix theProjectionMatrix) :
                             ComponentVisitor(),
                             list_(theList), _myScreenBB(theScreenBB),
                             projectionMatrix_(theProjectionMatrix) {
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
                if (myShapeWidget && myShapeWidget->getShape() && 
                    myShapeWidget->touches2DScreenCoords(_myScreenBB, projectionMatrix_)) {
                    RenderKey myKey(myShapeWidget->getWorldZ(), myShapeWidget->getShape()->isTransparent());
                    list_.push_back(std::make_pair(myShapeWidget, myKey));
                    DB(AC_TRACE << "CollectVisibleNodesVisitor collected: " << *myShapeWidget << ", worldZ: "
                             << myShapeWidget->getWorldZ() << " isTransparent: " << myShapeWidget->getShape()->isTransparent()
                             << "  current size " << list_.size();)
                }
                return true;
            }
        }
    }
    
    //XXX: seqfaults with sort, only works with stable sort, why?!
    bool
    sortByRenderKey(std::pair<ComponentPtr, RenderKey> i, std::pair<ComponentPtr, RenderKey> j) {
        if (i.second.transparency_ && j.second.transparency_) {
            return (i.second.z_ < j.second.z_);
        } else {
            return (!i.second.transparency_);
        }
    }

}

