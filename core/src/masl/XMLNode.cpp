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
        AC_PRINT << "print XML Node Tree";
        printXMLNode(node);
    }
}

