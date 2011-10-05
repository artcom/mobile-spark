#include "Component.h"

#include <masl/Logger.h>

namespace spark {

    Component::Component(): EventDispatcher(),_myRealizedFlag(false) {
    }

    Component::Component(const masl::XMLNodePtr theXMLNode):
        EventDispatcher(),
        _myXMLNode(theXMLNode),
        _myName(theXMLNode->name),
        _myParent(),
        _myRealizedFlag(false),
        _myRealizedAndAllChildrenFlag(false)
    {}

    Component::~Component() {
        AC_INFO << ".................delete " + _myName;
    }

    std::ostream &
    Component::print(std::ostream & os) const {
        os << getType() << " name: '" << getName() << "'";
        return os;
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
    
    void
    Component::realizeASync() {
        if (!_myRealizedFlag) {
            realize();
            _myRealizedFlag = true;
        } else {
            _myRealizedAndAllChildrenFlag = true;
            for (std::vector<ComponentPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
                if (!(*it)->isAllRealized()) {
                    (*it)->realizeASync();
                    _myRealizedAndAllChildrenFlag = false;
                    break;
                }
            }
        }
    }
}
