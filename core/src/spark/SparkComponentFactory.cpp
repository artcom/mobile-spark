#include "SparkComponentFactory.h"


#include <masl/Logger.h>
#include <masl/XMLUtils.h>
#include <mar/AssetProvider.h>

#include "Window.h"
#include "View.h"
#include "RenderCamera.h"
#include "World.h"
#include "Transform.h"
#include "Rectangle.h"
#include "Text.h"
#include "Image.h"
#include "Shape3D.h"
#include "Camera.h"
#include "NinePatch.h"

using namespace masl;

namespace spark {
    
    //TODO: decentralize this again
    void 
    SparkComponentFactory::setupFactory() {
        AC_PRINT << "SparkComponentFactory setup";
        bool registered = registerComponent("Window", spark::createWindow);
        registered = registerComponent(View::SPARK_TYPE, spark::createView);
        registered = registerComponent(RenderCamera::SPARK_TYPE, spark::createRenderCamera);
        registered = registerComponent("World", spark::createWorld);
        registered = registerComponent(Transform::SPARK_TYPE, spark::createTransform);
        registered = registerComponent("Rectangle", spark::createRectangle);
        registered = registerComponent("Image", spark::createImage);
        registered = registerComponent(Text::SPARK_TYPE, spark::createText);
        registered = registerComponent(Camera::SPARK_TYPE, spark::createCamera);
        registered = registerComponent("Shape3D", spark::createShape3D);
        registered = registerComponent("NinePatch", spark::createNinePatch);
        AC_PRINT << "SparkComponentFactory setup done";
    };
    
    SparkComponentFactory::SparkComponentFactory() {
        setupFactory();
    }
    SparkComponentFactory::~SparkComponentFactory() {
    }


    // Returns 'true' if registration was successful
    bool 
    SparkComponentFactory::registerComponent(const std::string & theComponentName,
                    const CreateComponentCallback theCreateFn) {
        return createCallbackMap_.insert(CallbackMap::value_type(theComponentName, theCreateFn)).second;
    }

    // Returns 'true' if the ShapeId was registered before
    bool 
    SparkComponentFactory::unregisterComponent(const std::string & theComponentName) {
        return createCallbackMap_.erase(theComponentName) == 1;
    }

    ComponentPtr 
    SparkComponentFactory::createComponent(const BaseAppPtr theApp, const XMLNodePtr theNode, ComponentPtr theParent) const {
        if (theNode->nodeName == "Template") {
            return ComponentPtr();
        }
        CallbackMap::const_iterator i;
        XMLNodePtr node = theNode;
        if (templateMap_.find(theNode->nodeName) != templateMap_.end()) {
            XMLNodePtr templateRoot = templateMap_.at(theNode->nodeName);
            //merge node
            for (std::map<std::string, std::string>::iterator it = theNode->attributes.begin(); it != theNode->attributes.end(); ++it) {
                templateRoot->attributes[it->first] = it->second;
            }
            i = createCallbackMap_.find(templateRoot->nodeName);
            node = templateRoot;
        } else {
            i = createCallbackMap_.find(theNode->nodeName);
        }
        if (i == createCallbackMap_.end()) {
            throw UnknownComponentException("Unknown Component Name: " + node->nodeName, PLUS_FILE_LINE);
        }
        return (i->second)(theApp, node, theParent);
    }

    ComponentPtr 
    SparkComponentFactory::loadSparkComponentsFromFile(const BaseAppPtr theApp, const std::string & thePath) {
        std::string myLayout = AssetProviderSingleton::get().ap()->getStringFromFile(thePath);
        return loadSparkComponentsFromString(theApp, myLayout);
    }

    ComponentPtr 
    SparkComponentFactory::loadSparkComponentsFromString(const BaseAppPtr theApp, const std::string & theNode) {
        XMLNodePtr myNode = prepareXMLFromString(theApp, theNode);
        ComponentPtr myComponentPtr = createComponent(theApp, myNode);
        AC_PRINT << "created component";
        return myComponentPtr;
    }

    XMLNodePtr
    SparkComponentFactory::loadXMLNodeFromFile(const BaseAppPtr theApp, const std::string & thePath) {
        std::string myLayout = AssetProviderSingleton::get().ap()->getStringFromFile(thePath);
        return loadXMLNodeFromString(theApp, myLayout);
    }

    XMLNodePtr
    SparkComponentFactory::loadXMLNodeFromString(const BaseAppPtr theApp, const std::string & theNode) {
        XMLNodePtr myNode = prepareXMLFromString(theApp, theNode);
        return myNode;
    }

    XMLNodePtr
    SparkComponentFactory::prepareXMLFromString(const BaseAppPtr theApp, const std::string & theNode) {
        xmlDocPtr doc = loadXMLFromMemory(theNode);
        xmlNode* myRootNode = xmlDocGetRootElement(doc);
        XMLNodePtr myNode(new XMLNode(myRootNode));
        resolveTemplates(theApp, myNode);
        xmlFreeDoc(doc);    //XXX    
        return myNode;
    }

    /*
     * assumptions: 
     * - Template nodes should be child nodes of root node of every file
     * - Templates should be included only once
     * - Templates should be included before they are used
     */
    void 
    SparkComponentFactory::resolveTemplates(const BaseAppPtr theApp, XMLNodePtr theRoot) {
        AC_PRINT << "......................... resolve templates";
        xmlNode* currentChild = theRoot->node->children;
        for (; currentChild; currentChild = currentChild->next) {
            if (currentChild->type == XML_ELEMENT_NODE && std::string(((const char*)currentChild->name)) == "Template") {
                std::string name;
                std::string source;
                xmlAttr *attribute = currentChild->properties;
                while (attribute) {
                    xmlNode* attrNode = attribute->children;
                    if (std::string((const char*)attribute->name) == "name") {
                        name = std::string((const char*)attrNode->content);
                    } else if (std::string((const char*)attribute->name) == "src") {
                        source = std::string((const char*)attrNode->content);
                    }
                    attribute = attribute->next;
                }
                AC_PRINT << currentChild << " name " << name << " source " << source;
                if (templateMap_.find(name) == templateMap_.end()) {
                    XMLNodePtr templateRoot = loadXMLNodeFromFile(theApp,source);
                    templateMap_[name] = templateRoot;
                    AC_PRINT << "inserted template " << name << " in templateMap";
                } else {
                    AC_ERROR << "template " << name << " was already defined";
                }
            }
        }
    }
}



