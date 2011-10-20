#include "Component.h"

#include <masl/Logger.h>

namespace spark {

    Component::Component(): EventDispatcher() {
    }

    Component::Component(const masl::XMLNodePtr theXMLNode):
        EventDispatcher(),
        _myXMLNode(theXMLNode),
        _myName(theXMLNode->name),
        _myParent(),
        _myRealizedAllChildrenFlag(false)
    {}

    Component::~Component() {
        AC_INFO << ".................delete " << _myName;
    }

    std::ostream &
    Component::print(std::ostream & os) const {
        os << getType() << " name: '" << getName() << "'";
        return os;
    }

    std::string 
    Component::getAttributesAsString() const {
        return " name=\""+_myName+"\"";
    }

    ComponentPtr
    Component::getChildByName(const std::string & theName, bool theDeepFlag) const {
        return ComponentPtr();
    }

    ComponentPtr
    Component::getRoot() {
        if (_myParent.lock()) {
            return _myParent.lock()->getRoot();
        } else {
            return shared_from_this();
        }
    }
    
    void
    Component::realizeASync() {
        _myRealizedAllChildrenFlag = true;
        for (std::vector<ComponentPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            if (!(*it)->isAllRealized()) {
                (*it)->realizeASync();
                _myRealizedAllChildrenFlag = false;
                break;
            }
        }
        if (_myRealizedAllChildrenFlag) {
            realize();
        }
    }
}
