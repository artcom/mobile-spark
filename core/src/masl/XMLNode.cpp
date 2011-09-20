#include "XMLNode.h"
#include "Logger.h"
#include "string_functions.h"
#include "numeric_functions.h"
#include "XMLUtils.h"

namespace masl {

    XMLNode::XMLNode(const std::string & theXMLString) {
        xmlDocPtr doc = loadXMLFromMemory(theXMLString);
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
                //AC_PRINT << "add child for " << nodeName << ": " << childXMLNode->nodeName;
                children.push_back(childXMLNode);
            } else if (currentChild->type == XML_TEXT_NODE) { 
                AC_PRINT << "TEXT this is not xml_element_node " << currentChild->name << " _ " << currentChild->content << " _ " << currentChild->line << "  " << currentChild->type << "  " << currentChild->properties;
                content = std::string((const char*)currentChild->content);
            } else if (currentChild->type == XML_CDATA_SECTION_NODE) { //cdata-sections
                AC_PRINT << "CDATA this is not xml_element_node " << currentChild->name << " _ " << currentChild->content << " _ " << currentChild->line << "  " << currentChild->type << "  " << currentChild->properties;

                xmlNode* innercurrentChild = currentChild->children;
                for (; innercurrentChild; innercurrentChild = innercurrentChild->next) {
                    AC_PRINT << "this is not xml_element_node " << innercurrentChild->name << "  " << innercurrentChild->content << " " << innercurrentChild->line << "  " << innercurrentChild->type << "  " << innercurrentChild->properties;
                }
                //TODO!!!
            } else {
                //AC_PRINT << "ELSE this is not xml_element_node " << currentChild->name << " _ " << currentChild->content << " _ " << currentChild->line << "  " << currentChild->type << "  " << currentChild->properties;
                //if (std::string((const char*)currentChild->name) == "text") {
                //    AC_PRINT << "try to get text ";
                //    xmlNode* innercurrentChild = currentChild->children;
                //    for (; innercurrentChild; innercurrentChild = innercurrentChild->next) {
                //        AC_PRINT << "this is not xml_element_node " << innercurrentChild->name << "  " << innercurrentChild->content << " " << innercurrentChild->line << "  " << innercurrentChild->type << "  " << innercurrentChild->properties;
                //    }
                //}
            }
        }
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
}

