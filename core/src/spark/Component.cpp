#include "Component.h"

namespace spark {

    Component::Component(): _myParent(ComponentPtr()) {
    }

    Component::Component(const XMLNodePtr theXMLNode, ComponentPtr theParent): 
        _myParent(theParent),
        _myXMLNode(theXMLNode),
        _myName(theXMLNode->name) {
    }

    Component::~Component() {
    }

    void Component::render() const {
    }

    void Component::drawObject() const {
    }
}
