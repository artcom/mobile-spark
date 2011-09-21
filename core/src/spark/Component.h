#ifndef _included_mobile_spark_Component_
#define _included_mobile_spark_Component_

#include <string>

#include <boost/smart_ptr/shared_ptr.hpp>

#include <masl/XMLNode.h>
#include <masl/MatrixStack.h>

#include "EventDispatcher.h"

using namespace masl;

namespace spark {

    class Component;
    typedef boost::shared_ptr<Component> ComponentPtr;
    typedef std::vector<ComponentPtr> VectorOfComponentPtr;

    class Component : public EventDispatcher, public boost::enable_shared_from_this<Component> {
    public:
        Component();
        Component(const XMLNodePtr theXMLNode, ComponentPtr theParent);
        virtual ~Component() = 0;
        
        std::ostream & print(std::ostream & os) const;
        friend inline std::ostream & operator<<(std::ostream & os, const Component & c) {
            return c.print(os);
        }

        virtual void realize() {};
        virtual void onPause() {};
        virtual void onResume() {};
        virtual void prerender(MatrixStack& theCurrentMatrixStack) {};
        virtual void render(const matrix & theProjectionMatrix) const {};

        const std::string & getName() const { return _myName; };
        const std::string & getNodeName() const { return _myXMLNode->nodeName; };
        virtual const char * const & getType() const = 0;
        const VectorOfComponentPtr & getChildren() { return _myChildren; };
        virtual ComponentPtr getChildByName(const std::string & theName, bool theDeepFlag = false) const;
        ComponentPtr getRoot();
        const ComponentPtr & getParent() const {return _myParent; };
        void setParent(ComponentPtr theParent) {_myParent = theParent;};
        virtual bool isRendered() const { return true;}

        virtual void testEvent(EventPtr theEvent) {AC_PRINT<<"event callback";};
    protected:
        const XMLNodePtr _myXMLNode;
        std::string _myName;
        VectorOfComponentPtr _myChildren;
    private:
        ComponentPtr _myParent;
    };
};
#endif
