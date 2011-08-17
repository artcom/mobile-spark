#include "SparkComponentFactory.h"


#include <masl/Logger.h>
#include <masl/XMLUtils.h>

using namespace masl;

namespace spark {

    // Returns 'true' if registration was successful
    bool SparkComponentFactory::registerComponent(const std::string & theComponentName,
                    const CreateComponentCallback theCreateFn) {
        return _myCreateCallbackMap.insert(CallbackMap::value_type(theComponentName, theCreateFn)).second;
    }

    // Returns 'true' if the ShapeId was registered before
    bool SparkComponentFactory::unregisterComponent(const std::string & theComponentName) {
        return _myCreateCallbackMap.erase(theComponentName) == 1;
    }

    ComponentPtr SparkComponentFactory::createComponent(const std::string & theComponentName, const BaseAppPtr theApp, const XMLNodePtr theNode, ComponentPtr theParent) const {
        CallbackMap::const_iterator i = _myCreateCallbackMap.find(theComponentName);
        if (i == _myCreateCallbackMap.end()) {
            throw std::runtime_error("Unknown Component Name");
        }
        return (i->second)(theApp, theNode, theParent);
    }

    ComponentPtr SparkComponentFactory::loadSparkLayout(const BaseAppPtr theApp, const std::string & thePath) {
        AC_PRINT << "in loadSparkLayout 1";

        std::string myLayout = theApp->assetProvider->getStringFromFile(thePath);
        
        AC_PRINT << "in loadSparkLayout 2";

        xmlDocPtr doc = loadXMLFromMemory(myLayout);
        
        AC_PRINT << "in loadSparkLayout 3";

        xmlNode* myRootNode = xmlDocGetRootElement(doc);
        AC_PRINT << "in loadSparkLayout 4";

        XMLNodePtr myNode(new XMLNode(myRootNode));
        
        AC_PRINT << "in loadSparkLayout 5";

        ComponentPtr myComponentPtr = createComponent(myNode->nodeName, theApp, myNode);
        AC_PRINT << "in loadSparkLayout 6";

        xmlFreeDoc(doc);        
        
        AC_PRINT << "in loadSparkLayout 7";

        return myComponentPtr;
    }
}
