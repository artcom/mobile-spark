// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "XMLNode.h"
#include "Logger.h"
#include "string_functions.h"
#include "numeric_functions.h"
#include "XMLUtils.h"

namespace masl {

    DEFINE_EXCEPTION(XMLNodeException, Exception);

    XMLNode::XMLNode(const std::string & theXMLString) {
        xmlDocPtr doc = loadXMLFromMemoryValidate(theXMLString);
        xmlNode* myRootNode = xmlDocGetRootElement(doc);
        init(myRootNode);
        xmlFreeDoc(doc);
    }

    XMLNode::XMLNode(xmlNode* theNode) {
        init(theNode);
    }

    XMLNode::~XMLNode() {
    }

    void
    XMLNode::init(xmlNode * theNode) {
        xmlAttr *attribute = theNode->properties;
        nodeName = (const char*)(theNode->name);
        while (attribute) {
            xmlNode* attrNode = attribute->children;
            attributes[std::string((const char*)attribute->name)] = std::string((const char*)attrNode->content);
            AC_TRACE << " attr " << std::string((const char*)attribute->name) << " = " <<  std::string((const char*)attrNode->content);
            attribute = attribute->next;
        }

        if (attributes.find("name") != attributes.end()) {
            name = attributes["name"];
        }

        xmlNode* currentChild = theNode->children;
        for (; currentChild; currentChild = currentChild->next) {
            if (currentChild->type == XML_ELEMENT_NODE) {
                XMLNodePtr childXMLNode = XMLNodePtr(new XMLNode(currentChild));
                AC_TRACE << "add child for " << nodeName << ": " << childXMLNode->nodeName;
                children.push_back(childXMLNode);
            } else if (currentChild->type == XML_TEXT_NODE) { 
                if (content.size() == 0) { //this is needed to avoid resetting content from CDATA by following TEXT
                    content = std::string((const char*)currentChild->content);
                    AC_TRACE << "set content for TEXT " << content;
                }
            } else if (currentChild->type == XML_CDATA_SECTION_NODE) { //cdata-sections
                content = std::string((const char*)currentChild->content);
                AC_TRACE << "set content for CDATA " << content;
            }
        }
    }

    std::ostream & 
    XMLNode::print(std::ostream & os) const {
        os << "<" << nodeName;
        for (std::map<std::string, std::string>::const_iterator it = attributes.begin(); it != attributes.end(); ++it) {
            os << " " << (*it).first << "='"<< (*it).second<<"'";
        }
        os << "/>";
        return os;
    }
}

