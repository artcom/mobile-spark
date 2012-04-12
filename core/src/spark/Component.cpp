// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "Component.h"

#include <masl/Logger.h>

namespace spark {

    Component::Component(): EventDispatcher() {
    }

    Component::Component(const masl::XMLNodePtr theXMLNode):
        EventDispatcher(),
        _myXMLNode(theXMLNode),
        _myName(theXMLNode->name),
        _myRealizedFlag(false)
    {}

    Component::~Component() {
        AC_TRACE << ".................delete " << _myName;
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
    Component::getRoot() {
        if (_myParent.lock()) {
            return _myParent.lock()->getRoot();
        } else {
            return shared_from_this();
        }
    }
    
    void
    Component::realizeASync() {
        realize();
    }
}
