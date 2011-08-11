#include "SparkComponentFactory.h"


#include "../base/Logger.h"
#include "Window.h"
#include "Transform.h"
#include "Rectangle.h"

using namespace asl;

//TODO: every new widget has to be
// * added to cmake
// * included here
// * added to the big if-elseif-...
// better ideas?

namespace spark {

    ComponentPtr SparkComponentFactory::createComponent(const BaseAppPtr theApp, const XMLNodePtr theNode, ComponentPtr theParent) {
        ComponentPtr component;
        if (theNode->nodeName == "Window") {
            AC_PRINT << "create Window";
            component = WindowPtr(new Window(theApp, theNode, theParent));
        } else if (theNode->nodeName == "Rectangle") {
            AC_PRINT <<"create Rectangle";
            component = RectanglePtr(new Rectangle(theApp, theNode, theParent));
        } else if (theNode->nodeName == "Transform") {
            AC_PRINT << "create Transform"; 
            component = TransformPtr(new Transform(theApp, theNode, theParent));
        }
        return component;
    }
}
