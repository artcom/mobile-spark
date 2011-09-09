#include "Container.h"

#include "SparkComponentFactory.h"

#include <masl/Logger.h>

using namespace masl;

namespace spark {

    Container::Container(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent)
        : Component(theXMLNode, theParent), _myApp(theApp)
    {
        AC_PRINT << "container constructor " << theXMLNode->nodeName;

        for (std::vector<XMLNodePtr>::iterator it = theXMLNode->children.begin(); it != theXMLNode->children.end(); ++it) {
            ComponentPtr childComponent = SparkComponentFactory::get().createComponent(_myApp, *it, ComponentPtr(this));
            if (childComponent) {
                _myChildren.push_back(childComponent);
            }
        }
    }

    Container::~Container() {
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

    ComponentPtr 
    Container::getChildByName(const std::string & theName, bool theDeepFlag) const {
        for (std::vector<ComponentPtr>::const_iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            if ((*it)->getName() == theName) {
                return (*it);
            } else {
                if (theDeepFlag) {
                    ComponentPtr myChildTraversal = (*it)->getChildByName(theName, theDeepFlag);
                    if (myChildTraversal) {
                        return myChildTraversal;
                    }
                }
            }
        }        
        return ComponentPtr();
    }

    void
    Container::removeChild(ComponentPtr theComponent) {
        theComponent->deleteParent();
        for (std::vector<ComponentPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            if (*it == theComponent) {
                _myChildren.erase(it);
                break;
            }
        }
    }
}
