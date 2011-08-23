#include "SparkComponentFactory.h"


#include <masl/Logger.h>
#include <masl/XMLUtils.h>

#include "Window.h"
#include "View.h"
#include "World.h"
#include "Transform.h"
#include "Rectangle.h"
#include "Image.h"

using namespace masl;

namespace spark {
    
    //TODO: decentralize this again
    void SparkComponentFactory::setupFactory() {
        AC_PRINT << "SparkComponentFactory setup";
        bool registered = registerComponent("Window", spark::createWindow);
        registered = registerComponent("View", spark::createView);
        registered = registerComponent("World", spark::createWorld);
        registered = registerComponent("Transform", spark::createTransform);
        registered = registerComponent("Rectangle", spark::createRectangle);

        registered = registerComponent("Image", spark::createImage);
        AC_PRINT << "SparkComponentFactory setup done";
    };
    
    SparkComponentFactory::SparkComponentFactory() {
        setupFactory();
    }
    
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
            throw std::runtime_error("Unknown Component Name: " + theComponentName);
        }
        return (i->second)(theApp, theNode, theParent);
    }

    ComponentPtr SparkComponentFactory::loadSparkLayout(const BaseAppPtr theApp, const std::string & thePath) {
        
        std::string myLayout = theApp->assetProvider->getStringFromFile(thePath);
        xmlDocPtr doc = loadXMLFromMemory(myLayout);
        xmlNode* myRootNode = xmlDocGetRootElement(doc);
        XMLNodePtr myNode(new XMLNode(myRootNode));
        ComponentPtr myComponentPtr = createComponent(myNode->nodeName, theApp, myNode);
        xmlFreeDoc(doc);        
        return myComponentPtr;
    }
}
