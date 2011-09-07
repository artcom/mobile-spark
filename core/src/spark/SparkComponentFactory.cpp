#include "SparkComponentFactory.h"


#include <masl/Logger.h>
#include <masl/XMLUtils.h>
#include <mar/AssetProvider.h>

#include "Window.h"
#include "View.h"
#include "Camera.h"
#include "World.h"
#include "Transform.h"
#include "Rectangle.h"
#include "Text.h"
#include "Image.h"
#include "Shape3D.h"

using namespace masl;

namespace spark {
    
    //TODO: decentralize this again
    void SparkComponentFactory::setupFactory() {
        AC_PRINT << "SparkComponentFactory setup";
        bool registered = registerComponent("Window", spark::createWindow);
        registered = registerComponent("View", spark::createView);
        registered = registerComponent("Camera", spark::createCamera);
        registered = registerComponent("World", spark::createWorld);
        registered = registerComponent("Transform", spark::createTransform);
        registered = registerComponent("Rectangle", spark::createRectangle);
        registered = registerComponent("Image", spark::createImage);
        registered = registerComponent("Text", spark::createText);
        registered = registerComponent("Shape3D", spark::createShape3D);
        AC_PRINT << "SparkComponentFactory setup done";
    };
    
    SparkComponentFactory::SparkComponentFactory() {
        setupFactory();
    }
    SparkComponentFactory::~SparkComponentFactory() {
    }


    // Returns 'true' if registration was successful
    bool SparkComponentFactory::registerComponent(const std::string & theComponentName,
                    const CreateComponentCallback theCreateFn) {
        return _myCreateCallbackMap.insert(CallbackMap::value_type(theComponentName, theCreateFn)).second;
    }

    // Returns 'true' if the component was registered before
    bool SparkComponentFactory::unregisterComponent(const std::string & theComponentName) {
        return _myCreateCallbackMap.erase(theComponentName) == 1;
    }

    ComponentPtr SparkComponentFactory::createComponent(const std::string & theComponentName, const BaseAppPtr theApp, const XMLNodePtr theNode, ComponentPtr theParent) const {
        AC_PRINT << "create component : " << theComponentName;
        CallbackMap::const_iterator i = _myCreateCallbackMap.find(theComponentName);
        if (i == _myCreateCallbackMap.end()) {
            throw UnknownComponentException("Unknown Component Name: " + theComponentName, PLUS_FILE_LINE);
        }
        return (i->second)(theApp, theNode, theParent);
    }

    ComponentPtr SparkComponentFactory::loadSparkLayoutFromFile(const BaseAppPtr theApp, const std::string & thePath) {
        std::string myLayout = AssetProviderSingleton::get().ap()->getStringFromFile(thePath);
        return loadSparkLayoutFromString(theApp, myLayout);
    }

    ComponentPtr SparkComponentFactory::loadSparkLayoutFromString(const BaseAppPtr theApp, const std::string & theNode) {
        xmlDocPtr doc = loadXMLFromMemory(theNode);
        xmlNode* myRootNode = xmlDocGetRootElement(doc);
        XMLNodePtr myNode(new XMLNode(myRootNode));
        ComponentPtr myComponentPtr = createComponent(myNode->nodeName, theApp, myNode);
        xmlFreeDoc(doc);        
        return myComponentPtr;
    }
}
