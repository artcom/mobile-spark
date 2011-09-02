#ifndef _included_mobile_spark_Visitors_
#define _included_mobile_spark_Visitors_

#include "Component.h"
#include "ShapeWidget.h"

namespace spark {

    class ComponentVisitor {
    public:
        ComponentVisitor() {};
        virtual ~ComponentVisitor();
        virtual void visit(ComponentPtr theComponent) = 0;
    };

    class PrintComponentVisitor : public ComponentVisitor {
    public:
        virtual void visit(ComponentPtr theComponent);
    };

    class OnPauseComponentVisitor : public ComponentVisitor {
    public:
        virtual void visit(ComponentPtr theComponent);
    };

    class OnResumeComponentVisitor : public ComponentVisitor {
    public:
        virtual void visit(ComponentPtr theComponent);
    };

    class CollectAABBComponentVisitor : public ComponentVisitor {
    public:
        CollectAABBComponentVisitor(std::vector<std::pair<ComponentPtr, float> > & theList, 
                                const float x, const float y,
                                const matrix theProjectionMatrix);
        virtual void visit(ComponentPtr theComponent);
    private:
        std::vector<std::pair<ComponentPtr, float> > & list_;
        //relative coordinates
        const float x_;
        const float y_;
        matrix projectionMatrix_;
    };


    template<class VISITOR> void 
    visitComponents(VISITOR & theVisitor, ComponentPtr theComponent) {
        theVisitor.visit(theComponent);
        if (theComponent->getChildren().size() == 0) { return; }
        for (std::vector<ComponentPtr>::const_iterator it = theComponent->getChildren().begin(); 
                                                       it != theComponent->getChildren().end(); ++it) {
            visitComponents(theVisitor, *it);
        }
    };
};

#endif
