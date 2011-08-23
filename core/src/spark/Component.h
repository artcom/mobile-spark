#ifndef _included_mobile_spark_Component_
#define _included_mobile_spark_Component_

#include <string>

#include <boost/smart_ptr/shared_ptr.hpp>

#include <masl/XMLNode.h>
#include <masl/MatrixStack.h>

using namespace masl;

namespace spark {

    class Component;
    typedef boost::shared_ptr<Component> ComponentPtr;

    class Component {
    public: 
        Component();
        Component(const XMLNodePtr theXMLNode, ComponentPtr theParent);
        virtual ~Component();
        virtual void render(MatrixStack& theCurrentMatrixStack, matrix theProjectionMatrix) const ;
        
        const std::string & getName() const { return _myName; };
        const std::string & getType() const;
        virtual ComponentPtr getChildByName(const std::string & theName) const; 
    protected:
        const XMLNodePtr _myXMLNode;
        std::string _myName;
        virtual void renderShape() const;
    private:
        ComponentPtr _myParent;
    };
};
#endif 
