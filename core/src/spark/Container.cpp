#include "Container.h"

#include "SparkComponentFactory.h"

namespace spark {

    Container::Container(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
            _myApp(theApp),
            Component(theXMLNode, theParent) {
        xmlNode* currentChild = theXMLNode->node->children;
        for (; currentChild; currentChild = currentChild->next) {
            if (currentChild->type == XML_ELEMENT_NODE) {
                XMLNodePtr childXMLNode = XMLNodePtr(new XMLNode(currentChild));
                _myChildren.push_back(createComponent(_myApp, childXMLNode, ContainerPtr(this)));
            }
        }
    }

    Container::~Container() {
    }
}
