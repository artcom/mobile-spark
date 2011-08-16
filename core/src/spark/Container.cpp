#include "Container.h"

#include "SparkComponentFactory.h"

namespace spark {

    Container::Container(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent)
        : Component(theXMLNode, theParent), _myApp(theApp)
    {

        xmlNode* currentChild = theXMLNode->node->children;
        for (; currentChild; currentChild = currentChild->next) {
            if (currentChild->type == XML_ELEMENT_NODE) {
                XMLNodePtr childXMLNode = XMLNodePtr(new XMLNode(currentChild));
                _myChildren.push_back(SparkComponentFactory::get().createComponent(childXMLNode->nodeName, _myApp, childXMLNode, ContainerPtr(this)));
            }
        }
    }

    Container::~Container() {
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
