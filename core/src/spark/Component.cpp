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

    void Component::render(MatrixStack& theCurrentMatrixStack, const matrix & theProjectionMatrix) const {
    }
    
    void Component::onPause() const  {
        AC_PRINT << "Spark::Component " << getName() << " onPause";
    }
    void Component::onResume() const  {
        AC_PRINT << "Spark::Component " << getName() << " onResume";
    }

    ComponentPtr Component::getChildByName(const std::string & theName, bool theDeepFlag) const {
        return ComponentPtr();
    }
    Component* Component::getRoot() {
        if (_myParent) {
            return _myParent->getRoot();
        } else {
            return this;
        }
    }
    
    const std::string & 
    Component::getType() const {
        return _myXMLNode->nodeName;
    }

}
