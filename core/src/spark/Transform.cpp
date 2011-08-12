#include "Transform.h"


namespace spark {
    Transform::Transform(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        Widget(theApp, theXMLNode, theParent) {
    }

    Transform::~Transform() {
    }
}
