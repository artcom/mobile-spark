#include "SparkComponentFactory.h"


#include <masl/Logger.h>
#include <masl/XMLUtils.h>
#include <mar/AssetProvider.h>


using namespace masl;
using namespace mar;

namespace spark {
    
    SparkComponentFactory::SparkComponentFactory() {
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
        CallbackMap::const_iterator i;
        if (templateMap_.find(theNode->nodeName) != templateMap_.end()) {
            XMLNodePtr templateRoot = templateMap_.at(theNode->nodeName);
            //merge node
            for (std::map<std::string, std::string>::iterator it = templateRoot->attributes.begin(); it != templateRoot->attributes.end(); ++it) {
                if (theNode->attributes.find(it->first) == theNode->attributes.end()) {
                    theNode->attributes[it->first] = it->second;
                }
            }
            for (std::vector<XMLNodePtr>::iterator it = templateRoot->children.begin(); it != templateRoot->children.end(); ++it) {
                theNode->children.push_back(*it);
            }
            if (theNode->name.size() > 0) {
                templateRoot->name = theNode->name;
            }
            theNode->nodeName = templateRoot->nodeName;
        }
        i = createCallbackMap_.find(theNode->nodeName);
        if (i == createCallbackMap_.end()) {
            AC_ERROR << "Unknown Component name: " << theNode->nodeName;
            throw UnknownComponentException("Unknown Component Name: " + theNode->nodeName, PLUS_FILE_LINE);
        }
        AC_DEBUG << "create component " << theNode->nodeName;
        return (i->second)(theApp, theNode, theParent);
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
        xmlFreeDoc(doc);
        resolveTemplates(theApp, myNode);
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
        for (std::vector<XMLNodePtr>::iterator it = theRoot->children.begin(); it != theRoot->children.end(); ++it) {
            if ((*it)->nodeName == "Template") {
                std::string name;
                std::string source;
                for (std::map<std::string, std::string>::iterator it2 = (*it)->attributes.begin(); it2 != (*it)->attributes.end(); ++it2) {
                    if (it2->first == "name") {
                        name = it2->second;
                    } else if (it2->first == "src") {
                        source = it2->second;
                    }
                }
                if (templateMap_.find(name) == templateMap_.end()) {
                    XMLNodePtr templateRoot = loadXMLNodeFromFile(theApp,source);
                    templateMap_[name] = templateRoot;
                }
            }
        }
    }
}



