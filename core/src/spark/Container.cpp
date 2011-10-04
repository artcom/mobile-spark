#include "Container.h"

#include <masl/Logger.h>

#include "SparkComponentFactory.h"

using namespace masl;

namespace spark {

    Container::Container(const BaseAppPtr theApp, const XMLNodePtr theXMLNode)
        : Component(theXMLNode), _myApp(theApp)
    {
        for (std::vector<XMLNodePtr>::iterator it = theXMLNode->children.begin(); it != theXMLNode->children.end(); ++it) {
            if ((*it)->nodeName == "Template") {
                continue;
            }
            ComponentPtr childComponent = SparkComponentFactory::get().createComponent(_myApp, *it);
            AC_DEBUG << "Container Constructor " << getName() << " add child " << childComponent->getName();
            _myChildren.push_back(childComponent);
        }
    }

    Container::~Container() {
    }

    void
    Container::realize() {
        Component::realize();
        for (std::vector<ComponentPtr>::const_iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            (*it)->setParent(shared_from_this());
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
    Container::addChild(const ComponentPtr theChild) {
        _myChildren.push_back(theChild);
        theChild->setParent(shared_from_this());
    }

    void
    Container::removeChild(ComponentPtr theComponent) {
        for (std::vector<ComponentPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            if (*it == theComponent) {
                _myChildren.erase(it);
                break;
            }
        }
    }

}
