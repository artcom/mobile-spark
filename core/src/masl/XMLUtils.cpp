#include "XMLUtils.h"

#include "XMLNode.h"
#include "Logger.h"

namespace masl {

    void printXMLNode(const xmlNode* theNode) {
        AC_PRINT << "print xml node";
        const xmlNode *cur_node = NULL;
        for (cur_node = theNode; cur_node; cur_node = cur_node->next) {
            AC_PRINT << " next loop";
            AC_PRINT << " type " << cur_node->type << " compare to " << XML_ELEMENT_NODE;
            if (cur_node->type == XML_ELEMENT_NODE) {
                AC_PRINT << "node type: Element, name: " << cur_node->name;
                xmlAttr *attribute = cur_node->properties;
                while (attribute) {
                    xmlNode* attrNode = attribute->children;
                    AC_PRINT << "attribute " << attribute->name << " = " << attrNode->content;
                    attribute = attribute->next;
                }
            }
            printXMLNode(cur_node->children);
        }
    }

    //XXX: the user should call xmlFreeDoc(doc)
    xmlDocPtr loadXML(const std::string & theFilename) {
        LIBXML_TEST_VERSION
        xmlParserCtxtPtr ctxt; /* the parser context */
        xmlDocPtr doc; /* the resulting document tree */

        /* create a parser context */
        ctxt = xmlNewParserCtxt();
        if (ctxt == NULL) {
            AC_ERROR << "Failed to allocate parser context";
        }
        /* parse the file, activating the DTD validation option */
        doc = xmlCtxtReadFile(ctxt, theFilename.c_str(), NULL, XML_PARSE_DTDATTR);

        /* check if parsing suceeded */
        if (doc == NULL) {
            AC_ERROR << "Failed to parse " << theFilename;
            return doc;
        } else {
        /* check if validation suceeded */
            if (ctxt->valid == 0) {  //does not validate, don't know why
                AC_ERROR << "Failed to validate " << theFilename;
            } else {
                AC_PRINT << "xml is valid";
            }
        }

        /* free up the resulting document */
        //xmlFreeDoc(doc);

        /* free up the parser context */
        xmlFreeParserCtxt(ctxt);
        xmlCleanupParser();
        xmlMemoryDump();

        return doc;
    }
}

