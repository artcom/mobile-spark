#include "XMLNode.h"
#include "Logger.h"
#include "string_functions.h"
#include "XMLUtils.h"

using namespace asl;
namespace cppcore {

    XMLNode::XMLNode(xmlNode* theNode) : 
        node(theNode) {
        xmlAttr *attribute = theNode->properties;
        nodeName = (const char*)(theNode->name);
        //AC_PRINT << "node name " << nodeName;
        while (attribute) {
            xmlNode* attrNode = attribute->children;
            attributes[std::string((const char*)attribute->name)] = std::string((const char*)attrNode->content);
            //AC_PRINT << " attr " << std::string((const char*)attribute->name) << " = " <<  std::string((const char*)attrNode->content);
            attribute = attribute->next;
        }
        //AC_PRINT << "finished properties of " << nodeName;

        if (attributes.find("name") != attributes.end()) {
            name = attributes["name"];
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

    std::vector<float> XMLNode::getFloatArrayValue(const std::string & theKey) const {
        std::map<std::string, std::string>::const_iterator it = attributes.find(theKey);
        std::vector<float> floatVector;
        if ( it != attributes.end()) {
            char* part = strtok(const_cast<char *>(it->second.c_str()), "[], ");
            while (part != NULL) {
                floatVector.push_back(stof(part));
                part = strtok(NULL, "[], ");
            }
        }
        return floatVector;
    }

    void XMLNode::print() const {
        AC_PRINT << "node " << name.c_str();
        for (std::map<std::string, std::string>::const_iterator it = attributes.begin(); it != attributes.end(); ++it) {
            AC_PRINT << "   " << (*it).first.c_str() << ": "<< (*it).second.c_str();
        }
    }

    void XMLNode::printTree() const {
        printXMLNode(node);
    }
}

