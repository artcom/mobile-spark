#include "XMLNode.h"
#include "Logger.h"
#include "string_functions.h"
#include "numeric_functions.h"
#include "XMLUtils.h"

namespace masl {

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

    vector4 XMLNode::getVector4Value(const std::string & theKey, const vector4 theDefault) const {
        vector4 myReturnValue = theDefault;
        std::vector<float> myValues = XMLNode::getFloatArrayValue(theKey);
        for (int i = 0; i < minimum(int(myValues.size()), 4); i++) {
            myReturnValue[i] = myValues[i];
        }
        return myReturnValue;
    }
    
    vector2 XMLNode::getVector2Value(const std::string & theKey, const vector2 theDefault) const {
        vector2 myReturnValue = theDefault;
        std::vector<float> myValues = XMLNode::getFloatArrayValue(theKey);
        for (int i = 0; i < minimum(int(myValues.size()), 2); i++) {
            myReturnValue[i] = myValues[i];
        }
        return myReturnValue;
    }

    std::vector<float> XMLNode::getFloatArrayValue(const std::string & theKey) const {
        std::map<std::string, std::string>::const_iterator it = attributes.find(theKey);
        std::vector<float> floatVector;
        if ( it != attributes.end()) {
            char* part = strtok(const_cast<char *>(it->second.c_str()), "[], ");
            while (part != NULL) {
                floatVector.push_back(as<float>(part));
                part = strtok(NULL, "[], ");
            }
        }
        return floatVector;
    }

    void XMLNode::print() const {
        std::ostringstream os;
        AC_PRINT << print(os);
    }

    std::ostream & XMLNode::print(std::ostream & os) const {
        os << "<" << nodeName;
        for (std::map<std::string, std::string>::const_iterator it = attributes.begin(); it != attributes.end(); ++it) {
            os << " " << (*it).first << "='"<< (*it).second<<"'";
        }
        os << "/>";
        return os;
    }

    void XMLNode::printTree() const {
        printXMLNode(node);
    }
}

