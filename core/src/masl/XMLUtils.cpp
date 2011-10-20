#include "XMLUtils.h"

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include "Logger.h"

namespace masl {

    DEFINE_EXCEPTION(XMLParsingException, Exception)

    void printXMLNode(const xmlNode* theNode) {
        AC_PRINT << "print xml node " << theNode->name;
        const xmlNode *cur_node = NULL;
        for (cur_node = theNode; cur_node; cur_node = cur_node->next) {
            AC_PRINT << " next loop " << cur_node->name;
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
        /*
        * this initialises the library and check potential ABI mismatches
        * between the version it was compiled for and the actual shared
        * library used.
        */
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
                AC_TRACE << "xml is valid";
            }
        }

        /* free up the resulting document */
        //xmlFreeDoc(doc);

        /* free up the parser context */
        xmlFreeParserCtxt(ctxt);
        return doc;
    }

    //XXX: the user should call xmlFreeDoc(doc)
    xmlDocPtr loadXMLFromMemoryValidate(const std::string & theXMLString) {
        /*
        * this initialises the library and check potential ABI mismatches
        * between the version it was compiled for and the actual shared
        * library used.
        */
        LIBXML_TEST_VERSION
        xmlParserCtxtPtr ctxt; /* the parser context */
        xmlDocPtr doc; /* the resulting document tree */

        /* create a parser context */
        ctxt = xmlNewParserCtxt();
        if (ctxt == NULL) {
            AC_ERROR << "Failed to allocate parser context";
        }
        /* parse the file, activating the DTD validation option */
        doc = xmlCtxtReadMemory(ctxt, theXMLString.c_str(), strlen(theXMLString.c_str()), "unused.xml", NULL, XML_PARSE_DTDATTR);

        /* check if parsing suceeded */
        if (doc == NULL) {
            AC_ERROR << "Failed to parse XMLString " << theXMLString;
            xmlErrorPtr myXMLError = xmlCtxtGetLastError(ctxt);
            xmlFreeParserCtxt(ctxt);
            throw XMLParsingException("Failed to parse XMLString: '" + std::string(myXMLError->message) + "'", PLUS_FILE_LINE); 
        } else {
        /* check if validation suceeded */
            if (ctxt->valid == 0) {  
                AC_ERROR << "Failed to validate XMLString " << theXMLString;
                xmlErrorPtr myXMLError = xmlCtxtGetLastError(ctxt);
                xmlFreeParserCtxt(ctxt);
                throw XMLParsingException("Failed to parse XMLString: '" + std::string(myXMLError->message) + "'", PLUS_FILE_LINE); 
            } else {
                AC_TRACE << "xml is valid";
            }
        }

        //XXX: doc is return value so no freeing here
        /* free up the resulting document */
        //xmlFreeDoc(doc);

        /* free up the parser context */
        xmlFreeParserCtxt(ctxt);
        return doc;
    }
    //
    //XXX: the user should call xmlFreeDoc(doc)
    xmlDocPtr 
    loadXMLFromMemory(const std::string & theXMLString) {
        /*
        * this initialises the library and check potential ABI mismatches
        * between the version it was compiled for and the actual shared
        * library used.
        */
        LIBXML_TEST_VERSION
        xmlDocPtr doc; /* the resulting document tree */
        
        //parse without context
        doc = xmlReadMemory(theXMLString.c_str(), strlen(theXMLString.c_str()), "unused.xml", NULL, 0);

        /* check if parsing suceeded */
        if (doc == NULL) {
            AC_ERROR << "Failed to parse XMLString (use loadXMLFromMemoryValidate to get more information)";
            throw XMLParsingException("Failed to parse XMLString", PLUS_FILE_LINE);
            return doc;
        }

        //XXX: doc is return value so no freeing here
        /* free up the resulting document */
        //xmlFreeDoc(doc);
        return doc;
    }

    std::vector<XMLNodePtr> 
    loadXMLFromMemoryWithXPath(const std::string & theXMLString, const std::string & theXPath) {
        LIBXML_TEST_VERSION
        std::vector<XMLNodePtr> result;
#if defined(LIBXML_XPATH_ENABLED) && defined(LIBXML_SAX1_ENABLED)
        xmlDocPtr doc;
        xmlXPathContextPtr xpathCtx; 
        xmlXPathObjectPtr xpathObj; 

        //parse without context
        doc = xmlReadMemory(theXMLString.c_str(), strlen(theXMLString.c_str()), "unused.xml", NULL, 0);
        if (doc == NULL) {
            AC_ERROR << "Failed to parse XMLString (use loadXMLFromMemoryValidate to get more information)";
            throw XMLParsingException("Failed to parse XMLString", PLUS_FILE_LINE);
            return result;
        }

        xpathCtx = xmlXPathNewContext(doc);
        if(xpathCtx == NULL) {
            AC_ERROR << "Error: unable to create new XPath context";
            xmlFreeDoc(doc); 
            return result;
        }
    
        xpathObj = xmlXPathEvalExpression((const xmlChar *)theXPath.c_str(), xpathCtx);
        if(xpathObj == NULL) {
            AC_ERROR << "Error: unable to evaluate xpath expression " << theXPath;
            xmlXPathFreeContext(xpathCtx); 
            xmlFreeDoc(doc); 
            return result;
        }

        xmlNodeSetPtr nodeSet = xpathObj->nodesetval;
        size_t size = (nodeSet ? nodeSet->nodeNr : 0);
        for (size_t i = 0; i < size; ++i) {
            result.push_back(XMLNodePtr(new XMLNode(nodeSet->nodeTab[i])));
        }

        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx); 
        xmlFreeDoc(doc); 
#endif
        return result;
    }
}

