#include "XMLUtils.h"

#include "XMLNode.h"
//#include "spark/SparkComponentFactory.h"
//#include "spark/Widget.h"

#include "log.h"

namespace cppcore {

    void printXMLNode(const xmlNode* theNode) {
        const xmlNode *cur_node = NULL;
        for (cur_node = theNode; cur_node; cur_node = cur_node->next) {
            if (cur_node->type == XML_ELEMENT_NODE) {
                LOGI("node type: Element, name: %s\n", cur_node->name);
                xmlAttr *attribute = cur_node->properties;
                while (attribute) {
                    xmlNode* attrNode = attribute->children;
                    LOGI("attribute %s = %s\n",attribute->name, attrNode->content);
                    attribute = attribute->next;
                }
            }
            printXMLNode(cur_node->children);
        }
    }

    void loadLayout(const std::string & theFilename) {
        LOGI("-------------------before parsing");
        LIBXML_TEST_VERSION
        xmlParserCtxtPtr ctxt; /* the parser context */
        xmlDocPtr doc; /* the resulting document tree */
        xmlNode *root_element = NULL;

        /* create a parser context */
        ctxt = xmlNewParserCtxt();
        if (ctxt == NULL) {
            LOGE("Failed to allocate parser context\n");
        }
        /* parse the file, activating the DTD validation option */
        doc = xmlCtxtReadFile(ctxt, theFilename.c_str(), NULL, XML_PARSE_DTDATTR);
        /* check if parsing suceeded */
        if (doc == NULL) {
            LOGE("Failed to parse %s\n", theFilename.c_str());
            return;
        } else {
        /* check if validation suceeded */
            if (ctxt->valid == 0) {  //does not validate, don't know why
                LOGE("Failed to validate %s\n", theFilename.c_str());
            } else {
                LOGI("xml is valid");
            }

        }

        root_element = xmlDocGetRootElement(doc);
        printXMLNode(root_element);
        LOGI("------------ try to create XMLNode");
        XMLNode* myNode = new XMLNode(root_element);
        myNode->print();
        //spark::Component* myRootWidget = spark::SparkComponentFactory::createComponent(myNode);


        /* free up the resulting document */
        xmlFreeDoc(doc);

        /* free up the parser context */
        xmlFreeParserCtxt(ctxt);
        xmlCleanupParser();
        xmlMemoryDump();
        LOGI("-------------------after parsing");
    }
}

