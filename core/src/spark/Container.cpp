#include "Container.h"

#include "SparkComponentFactory.h"

#include <masl/Logger.h>

using namespace masl;

namespace spark {

    Container::Container(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent)
        : Component(theXMLNode, theParent), _myApp(theApp)
    {

        xmlNode* currentChild = theXMLNode->node->children;
        for (; currentChild; currentChild = currentChild->next) {
            if (currentChild->type == XML_ELEMENT_NODE) {
                XMLNodePtr childXMLNode = XMLNodePtr(new XMLNode(currentChild));
                _myChildren.push_back(SparkComponentFactory::get().createComponent(childXMLNode->nodeName, _myApp, childXMLNode, ComponentPtr(this)));
            }
        }
    }

    Container::~Container() {
    }
    void Container::onPause() const  {
        Component::onPause();
        AC_PRINT << "Spark::Container " << getName() << " onPause";
        for (std::vector<ComponentPtr>::const_iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            (*it)->onPause();
        }
        
    }
    void Container::onResume() const  {
        Component::onResume();
        AC_PRINT << "Spark::Container " << getName() << " onResume";
        for (std::vector<ComponentPtr>::const_iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            (*it)->onResume();
        }
    }
    
    VectorOfComponentPtr 
    Container::getChildrenByType(const std::string & theType) const {
        VectorOfComponentPtr myResult;
        for (std::vector<ComponentPtr>::const_iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            if ((*it)->getType() == theType) {
                myResult.push_back(*it);
            }
        }        
        return myResult;
    }

    ComponentPtr Container::getChildByName(const std::string & theName) const {
        for (std::vector<ComponentPtr>::const_iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            if ((*it)->getName() == theName) {
                return (*it);
            }
        }
        return ComponentPtr();
    }
}
