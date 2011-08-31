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
    class Container;
    typedef boost::shared_ptr<Container> ContainerPtr;

    class Component : public EventDispatcher, public boost::enable_shared_from_this<Component> {
    public: 
        Component();
        Component(const XMLNodePtr theXMLNode, ComponentPtr theParent);
        virtual ~Component() = 0;
        virtual void render(MatrixStack& theCurrentMatrixStack, const matrix & theProjectionMatrix) const ;
        virtual void onPause() const ;
        virtual void onResume() const ;
        
        virtual void testEvent(EventPtr theEvent) {AC_PRINT<<"event callback";};
        const std::string & getName() const { return _myName; };
        const std::string & getType() const;
        virtual ComponentPtr getChildByName(const std::string & theName, bool theDeepFlag = false) const;
        Component* getRoot();
        const ComponentPtr & getParent() const {return _myParent; };
        void insertAtParent(ContainerPtr theParent);
    protected:
        const XMLNodePtr _myXMLNode;
        std::string _myName;
    private:
        ComponentPtr _myParent;
    };
};
#endif 
