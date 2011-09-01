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
        _myParent(theParent) {
    }

    Component::~Component() {
        AC_PRINT << ".................delete " + _myName;
    }

    void 
    Component::render(MatrixStack& theCurrentMatrixStack, const matrix & theProjectionMatrix) const {
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
    
    const std::string & 
    Component::getType() const {
        return _myXMLNode->nodeName;
    }

    void 
    Component::insertAtParent(ContainerPtr theParent) {
        _myParent = theParent;
        theParent->insertChild(shared_from_this()); 
    }
}
