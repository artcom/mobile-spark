#include "XMLNode.h"
#include <sstream>
#include "XMLUtils.h"
#include "Logger.h"
#include "StringHelper.h"
//#include "ndk3Dlib.h"
//#include "def.h"

namespace cppcore {

    XMLNode::XMLNode(xmlNode* theNode) : _myNode(theNode) {
        name = (const char*)(theNode->name);
        xmlAttr *attribute = theNode->properties;
        while (attribute) {
            xmlNode* attrNode = attribute->children;
            attributes[std::string((const char*)attrNode->name)] = std::string((const char*)attrNode->content);
            attribute = attribute->next;
        }

        xmlNode* currentChild = theNode->children;
        for (; currentChild; currentChild = currentChild->next) {
            if (currentChild->type == XML_ELEMENT_NODE) {
                children.push_back(new XMLNode(currentChild));
            }
        }
    }

    XMLNode::~XMLNode() {
    }

    float XMLNode::getFloatValue(const std::string & theKey, const float theDefault) const {
        std::map<std::string, std::string>::const_iterator it = attributes.find(theKey);
        if ( it != attributes.end()) {
            return stof(it->second);
        } else {
            return theDefault;
        }
    }

    void XMLNode::print() const {
        AC_PRINT << "node " << name.c_str();
        for (std::map<std::string, std::string>::const_iterator it = attributes.begin(); it != attributes.end(); ++it) {
            AC_PRINT << "   " << (*it).first.c_str() << ": "<< (*it).second.c_str();
        }
    }

    void XMLNode::printTree() const {
        printXMLNode(_myNode);
    }
}

