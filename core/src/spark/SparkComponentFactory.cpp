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

    void SparkComponentFactory::createComponent(ComponentPtr& theReturn, const BaseAppPtr theApp, const XMLNodePtr theNode, ComponentPtr theParent) {
        if (theNode->nodeName == "Window") {
            AC_PRINT << "create Window";
            theReturn = WindowPtr(new Window(theApp, theNode, theParent));
        } else if (theNode->nodeName == "Rectangle") {
            AC_PRINT <<"create Rectangle";
            theReturn = RectanglePtr(new Rectangle(theApp, theNode, theParent));
        } else if (theNode->nodeName == "Transform") {
            AC_PRINT << "create Transform"; 
            theReturn = TransformPtr(new Transform(theApp, theNode, theParent));
        }
    }


    ComponentPtr SparkComponentFactory::loadSparkLayout(const BaseAppPtr theApp, const std::string & thePath) {
        ComponentPtr myComponentPtr;
        xmlDocPtr doc = loadXML(thePath);
        xmlNode* myRootNode = xmlDocGetRootElement(doc);
        XMLNodePtr myNode(new XMLNode(myRootNode));
        createComponent(myComponentPtr, theApp, myNode);
        xmlFreeDoc(doc);        
        return myComponentPtr;
    }
}
