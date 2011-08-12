#include "SparkComponentFactory.h"


#include <masl/Logger.h>
#include <masl/XMLUtils.h>

#include "Window.h"
#include "Transform.h"
#include "Rectangle.h"

using namespace masl;

//TODO: every new widget has to be
// * included here
// * added to the big if-elseif-...
// better ideas?

namespace spark {

    ComponentPtr SparkComponentFactory::createComponent(const BaseAppPtr theApp, const XMLNodePtr theNode, ComponentPtr theParent) {
        ComponentPtr theReturn;
        if (theNode->nodeName == "Window") {
            //AC_INFO << "create Window";
            theReturn = WindowPtr(new Window(theApp, theNode, theParent));
        } else if (theNode->nodeName == "Rectangle") {
            //AC_INFO <<"create Rectangle";
            theReturn = RectanglePtr(new Rectangle(theApp, theNode, theParent));
        } else if (theNode->nodeName == "Transform") {
            //AC_INFO << "create Transform"; 
            theReturn = TransformPtr(new Transform(theApp, theNode, theParent));
        }
        return theReturn;
    }


    ComponentPtr SparkComponentFactory::loadSparkLayout(const BaseAppPtr theApp, const std::string & thePath) {
        std::string myLayout = theApp->assetProvider->getStringFromFile(thePath);
        xmlDocPtr doc = loadXMLFromMemory(myLayout);
        xmlNode* myRootNode = xmlDocGetRootElement(doc);
        XMLNodePtr myNode(new XMLNode(myRootNode));
        ComponentPtr myComponentPtr;
        myComponentPtr = createComponent(theApp, myNode);
        xmlFreeDoc(doc);        
        return myComponentPtr;
    }
}
