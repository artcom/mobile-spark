#ifndef _included_mobile_spark_Visitors_
#define _included_mobile_spark_Visitors_

#include "Component.h"
#include "ShapeWidget.h"

namespace spark {

    class ComponentVisitor {
    public:
        ComponentVisitor() {};
        virtual ~ComponentVisitor();
        virtual bool visit(ComponentPtr theComponent) = 0;
        virtual bool preCheck(ComponentPtr theComponent) { return true; };
    };

    class PrintComponentVisitor : public ComponentVisitor {
    public:
        virtual bool visit(ComponentPtr theComponent);
    };

    class RealizeComponentsButWorldAndWindowVisitor : public ComponentVisitor {
    public:
        virtual bool visit(ComponentPtr theComponent);
        virtual bool preCheck(ComponentPtr theComponent);
    };
    
    class OnPauseComponentVisitor : public ComponentVisitor {
    public:
        virtual bool visit(ComponentPtr theComponent);
    };
    
    class OnResumeComponentVisitor : public ComponentVisitor {
    public:
        virtual bool visit(ComponentPtr theComponent);
    };

    class ReparentComponentVisitor : public ComponentVisitor {
    public:
        virtual bool visit(ComponentPtr theComponent);
    };
    class CollectAABBComponentVisitor : public ComponentVisitor {
    public:
        CollectAABBComponentVisitor(std::vector<std::pair<ComponentPtr, float> > & theList,
                                const float x, const float y,
                                const matrix theProjectionMatrix);
        virtual bool visit(ComponentPtr theComponent);
    private:
        std::vector<std::pair<ComponentPtr, float> > & list_;
        //relative coordinates
        const float x_;
        const float y_;
        matrix projectionMatrix_;
    };

    struct RenderKey {
        RenderKey(float theZ, bool theTransparency) : z_(theZ), transparency_(theTransparency) {};
        float z_;
        bool transparency_;
    };

    typedef std::vector<std::pair<ComponentPtr, RenderKey> > RenderList;

    class CollectVisibleNodesVisitor : public ComponentVisitor {
    public:
        CollectVisibleNodesVisitor(RenderList & theList);
        virtual bool visit(ComponentPtr theComponent);
    private:
        RenderList & list_;
    };


    template<class VISITOR> void
    parentFirstVisitComponents(VISITOR & theVisitor, ComponentPtr theComponent) {
        bool myContinueTraversal = theVisitor.visit(theComponent);
        if (!myContinueTraversal || theComponent->getChildren().size() == 0) { return; }
        for (std::vector<ComponentPtr>::const_iterator it = theComponent->getChildren().begin();
                                                       it != theComponent->getChildren().end(); ++it) {
            parentFirstVisitComponents(theVisitor, *it);
        }
    };

    template<class VISITOR> void
    childFirstVisitComponents(VISITOR & theVisitor, ComponentPtr theComponent) {
        if (theVisitor.preCheck(theComponent)) {
            for (std::vector<ComponentPtr>::const_iterator it = theComponent->getChildren().begin();
                                                           it != theComponent->getChildren().end(); ++it) {
                childFirstVisitComponents(theVisitor, *it);
            }
            theVisitor.visit(theComponent);
        }
    };

    bool sortByRenderKey(std::pair<ComponentPtr, RenderKey> i, std::pair<ComponentPtr, RenderKey> j);
};

#endif
