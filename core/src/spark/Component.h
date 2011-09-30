#ifndef _included_mobile_spark_Component_
#define _included_mobile_spark_Component_

#include <string>

#include <masl/Ptr.h>
#include <masl/XMLNode.h>
#include <masl/MatrixStack.h>

#include "EventDispatcher.h"

namespace spark {

    class Component;
    typedef masl::Ptr<Component> ComponentPtr;
    typedef std::vector<ComponentPtr> VectorOfComponentPtr;

    class Component : public EventDispatcher, public boost::enable_shared_from_this<Component> {
    public:
        Component();
        Component(const masl::XMLNodePtr theXMLNode);
        virtual ~Component() = 0;
        
        std::ostream & print(std::ostream & os) const;
        friend inline std::ostream & operator<<(std::ostream & os, const Component & c) {
            return c.print(os);
        }

        virtual void realize() {};
        void realizeASync();
        const bool isAllRealized() const { return _myRealizedAndAllChildrenFlag;}
        virtual void onPause() {};
        virtual void onResume() {};
        virtual void prerender(MatrixStack& theCurrentMatrixStack) {};
        virtual void render(const matrix & theProjectionMatrix) const {};

        const std::string & getName() const { return _myName; };
        virtual const char * const & getType() const = 0;
        const VectorOfComponentPtr & getChildren() const { return _myChildren; }; //XXX: should be in Container
        virtual ComponentPtr getChildByName(const std::string & theName, bool theDeepFlag = false) const;
        ComponentPtr getRoot();
        const masl::XMLNodePtr getNode() const { return _myXMLNode; }
        const ComponentPtr & getParent() const { return _myParent; };
        void setParent(ComponentPtr theParent) {_myParent = theParent;};
        virtual bool isRendered() const { return true;}

    protected:
        const masl::XMLNodePtr _myXMLNode;
        std::string _myName;
        VectorOfComponentPtr _myChildren;
    private:
        ComponentPtr _myParent;
        bool _myRealizedFlag;
        bool _myRealizedAndAllChildrenFlag;
    };
};
#endif
