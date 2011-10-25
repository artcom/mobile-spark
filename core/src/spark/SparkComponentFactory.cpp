// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "SparkComponentFactory.h"

#include <set>

#include <masl/Logger.h>
#include <masl/XMLUtils.h>
#include <masl/AssetProvider.h>

#include "Visitors.h"


using namespace masl;
using namespace mar;

namespace spark {

    DEFINE_EXCEPTION(SparkComponentException, masl::Exception)
    DEFINE_EXCEPTION(UnknownComponentException, SparkComponentException)

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
    SparkComponentFactory::createComponent(const BaseAppPtr theApp, const XMLNodePtr theNode) const {
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
        return (i->second)(theApp, theNode);
    }

    ComponentPtr
    SparkComponentFactory::loadSparkComponentsFromFile(const BaseAppPtr theApp, const std::string & thePath,
                                                       const ContainerPtr theParent) {
        std::string myLayout = masl::AssetProviderSingleton::get().ap()->getStringFromFile(thePath);
        return loadSparkComponentsFromString(theApp, myLayout, theParent);
    }

    ComponentPtr
    SparkComponentFactory::loadSparkComponentsFromString(const BaseAppPtr theApp, const std::string & theNode, 
                                                         const ContainerPtr theParent) {
        XMLNodePtr myNode(new XMLNode(theNode));
        resolveTemplates(theApp, myNode);
        ComponentPtr myComponentPtr = createComponent(theApp, myNode);
        if (theParent) {
            theParent->addChild(myComponentPtr);
        }
        ReparentComponentVisitor myReparentVisitor;
		parentFirstVisitComponents(myReparentVisitor, myComponentPtr);
        RealizeComponentsButWorldAndWindowVisitor myVisitor;
		childFirstVisitComponents(myVisitor, myComponentPtr);
        return myComponentPtr;
    }

    XMLNodePtr
    SparkComponentFactory::loadXMLNodeFromFile(const BaseAppPtr theApp, const std::string & thePath) {
        std::string myLayout = masl::AssetProviderSingleton::get().ap()->getStringFromFile(thePath);
        return loadXMLNodeFromString(theApp, myLayout);
    }

    XMLNodePtr
    SparkComponentFactory::loadXMLNodeFromString(const BaseAppPtr theApp, const std::string & theNode) {
        XMLNodePtr myNode(new XMLNode(theNode));
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

    std::vector<std::string> 
    SparkComponentFactory::createSrcListFromSpark(const std::string & theSparkString) {
        //TODO: this should be done with xpath as soon as libxml2 is compiled with xpath support
        //assumptions: file names should not contain escaped quotes
        std::vector<std::string> result;
        std::set<std::string> set;
        size_t startPos = 0;
        size_t foundPos;
        while (std::string::npos != (foundPos = theSparkString.find("src=\"",startPos))) {
            startPos = foundPos + 5;
            foundPos = theSparkString.find("\"",startPos);
            if (foundPos == std::string::npos) {
                return result;
            }
            set.insert(theSparkString.substr(startPos, foundPos - startPos));
            startPos = foundPos + 1;
        }
        result = std::vector<std::string>(set.begin(), set.end());
        return result;
    }
}



