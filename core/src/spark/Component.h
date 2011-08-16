#ifndef _included_mobile_spark_Component_
#define _included_mobile_spark_Component_

#include <string>

#include <boost/smart_ptr/shared_ptr.hpp>

#include <masl/XMLNode.h>

using namespace masl;

namespace spark {

    class Component;
    typedef boost::shared_ptr<Component> ComponentPtr;

    class Component {
    public: 
        Component();
        Component(const XMLNodePtr theXMLNode, ComponentPtr theParent);
        virtual ~Component();
        virtual void render() const ;
        
    protected:
        const XMLNodePtr _myXMLNode;
        std::string _myName;
        virtual void renderShape() const;

    private:
        ComponentPtr _myParent;
    };
};
#endif 
