#include "Component.h"

#include <masl/Logger.h>

namespace spark {

    Component::Component(): _myParent(ComponentPtr()) {
    }

    Component::Component(const XMLNodePtr theXMLNode, ComponentPtr theParent): 
        _myXMLNode(theXMLNode),
        _myName(theXMLNode->name),
        _myParent(theParent) {
    }

    Component::~Component() {
        AC_PRINT << "delete " + _myName;
    }

    void Component::render(MatrixStack& theCurrentMatrixStack, matrix theProjectionMatrix) const {
    }

    void Component::renderShape() const {
    }

    ComponentPtr Component::getChildByName(const std::string & theName) const {
        return ComponentPtr();
    }
    
    const std::string & 
    Component::getType() const {
        return _myXMLNode->nodeName;
    }

}
