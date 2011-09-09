#include "Container.h"

#include "SparkComponentFactory.h"

#include <masl/Logger.h>

using namespace masl;

namespace spark {

    Container::Container(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent)
        : Component(theXMLNode, theParent), _myApp(theApp)
    {
        AC_PRINT << "container constructor " << theXMLNode->nodeName;

        xmlNode* currentChild = theXMLNode->node->children;
        for (; currentChild; currentChild = currentChild->next) {
            AC_PRINT << "process child of container";
            if (currentChild->type == XML_ELEMENT_NODE) {
                XMLNodePtr childXMLNode = XMLNodePtr(new XMLNode(currentChild));
                AC_PRINT << " constructor child loop of " << getName() << " child nodeType " << childXMLNode->nodeName;
                ComponentPtr childComponent = SparkComponentFactory::get().createComponent(_myApp, childXMLNode, ComponentPtr(this));
                AC_PRINT << " 1";
                if (childComponent) {
                    _myChildren.push_back(childComponent);
                    AC_PRINT << "pushed back";
                } else {
                    AC_PRINT << "child is null";
                }
            } else {
                AC_PRINT << "no xml node " << currentChild->type;
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
