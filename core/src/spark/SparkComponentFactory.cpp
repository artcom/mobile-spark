#include "SparkComponentFactory.h"


#include "../base/Logger.h"
#include "../base/XMLUtils.h"

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

    ComponentPtr createComponent(const BaseAppPtr theApp, const XMLNodePtr theNode, ComponentPtr theParent) {
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


    ComponentPtr loadSparkLayout(const BaseAppPtr theApp, const std::string & thePath) {

        AC_PRINT << "hello from factory";
        ComponentPtr myComponentPtr;
        AC_PRINT << "1";
        xmlNode* myRootNode = loadXML(thePath);
        AC_PRINT << "2";
        AC_PRINT << " good root? " << myRootNode;
        AC_PRINT << " name " << myRootNode->name;
        AC_PRINT << " type " << myRootNode->type;

        //XMLNode* help = new XMLNode(myRootNode);
        AC_PRINT << "2.5";
        //XMLNodePtr myNode(help);
        AC_PRINT << "3";
        //myComponentPtr =  createComponent(theApp, myNode);
        AC_PRINT << "4";
        return myComponentPtr;
    }
}
