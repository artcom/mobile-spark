#include "Component.h"

#include <masl/Logger.h>

#include "Container.h"

namespace spark {

    Component::Component(): EventDispatcher(), _myParent(ComponentPtr()) {
    }

    Component::Component(const XMLNodePtr theXMLNode, ComponentPtr theParent):
        EventDispatcher(),
        _myXMLNode(theXMLNode),
        _myName(theXMLNode->name),
        _myParent(theParent)
    {}

    Component::~Component() {
        AC_PRINT << ".................delete " + _myName;
    }

    ComponentPtr
    Component::getChildByName(const std::string & theName, bool theDeepFlag) const {
        return ComponentPtr();
    }

    ComponentPtr
    Component::getRoot() {
        if (_myParent) {
            return _myParent->getRoot();
        } else {
            return shared_from_this();
        }
    }
}
