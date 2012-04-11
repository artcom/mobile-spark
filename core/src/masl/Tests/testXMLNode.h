// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_asl_test_XMLNode_h_included_
#define _ac_mobile_asl_test_XMLNode_h_included_

#include <string>
#include "../UnitTest.h"
#include "../XMLNode.h"
#include "../XMLUtils.h"


namespace masl {
    class XMLNode_UnitTest : public UnitTest {
        public:
            XMLNode_UnitTest() : UnitTest("XMLNode_UnitTest") {  }
            ~XMLNode_UnitTest() {
                xmlCleanupParser();
                xmlMemoryDump();
            }

            void run() {
                perform_XMLLibTest();
                perform_XMLNodeTest();
                perform_loadXMLTest();
                perform_loadMultipleXMLTest();
            }

            //fixtures
            static const char *include;
            static const char *sparkFile;

            void perform_XMLLibTest() {
                xmlParserCtxtPtr ctxt;
                xmlDocPtr doc;
                ctxt = xmlNewParserCtxt();
                ENSURE_MSG(ctxt, "parser context should not be null");
                doc = xmlCtxtReadMemory(ctxt, include, strlen(include), "include.xml", NULL, XML_PARSE_DTDATTR);
                ENSURE_MSG(doc,"xml doc should not be null");
                ENSURE_MSG(ctxt->valid, "xml should be valid");
                xmlNode *root_node = NULL;
                root_node = xmlDocGetRootElement(doc);
                ENSURE_MSG(root_node,"root node should not be null");
                ENSURE_EQUAL(std::string((const char*)root_node->name), std::string("document"));
                xmlFreeDoc(doc);
                xmlFreeParserCtxt(ctxt);
            }

            void perform_XMLNodeTest() {
                xmlParserCtxtPtr ctxt;
                xmlDocPtr doc;
                ctxt = xmlNewParserCtxt();
                doc = xmlCtxtReadMemory(ctxt, sparkFile, strlen(sparkFile), "layout.spark", NULL, XML_PARSE_DTDATTR);
                xmlNode *root_node = NULL;
                root_node = xmlDocGetRootElement(doc);
                ENSURE_MSG(root_node,"root_node should not be null");
                XMLNode *myXMLNode = new XMLNode(root_node);
                ENSURE_MSG(myXMLNode, "XMLNode should not be null");
                XMLNodePtr myXMLNode2(new XMLNode(root_node));
                ENSURE_MSG(myXMLNode2, "XMLNode should not be null");
                ENSURE_EQUAL(myXMLNode2->nodeName, std::string("Window"));
                ENSURE_EQUAL(myXMLNode2->name, std::string("ourWindow"));
                ENSURE_MSG(myXMLNode2->attributes.find("width") != myXMLNode2->attributes.end(), "width should be in attributes of document");
                ENSURE_EQUAL(myXMLNode2->attributes["width"], std::string("300"));
                xmlFreeDoc(doc);
                xmlFreeParserCtxt(ctxt);
            }

            void perform_loadXMLTest() {
                xmlDocPtr doc = loadXML("/sdcard/test.spark");
                ENSURE_MSG(doc, "doc should not be null");
                xmlNode* xmlLibNode = xmlDocGetRootElement(doc);
                ENSURE_MSG(xmlLibNode, "xmlLibNode should not be null");
                XMLNode *myXMLNode = new XMLNode(xmlLibNode);
                ENSURE_MSG(myXMLNode, "XMLNode should not be null");
                XMLNodePtr myXMLNode2(new XMLNode(xmlLibNode));
                ENSURE_MSG(myXMLNode2, "XMLNode should not be null");
                ENSURE_EQUAL(myXMLNode2->nodeName, std::string("Window"));
                ENSURE_EQUAL(myXMLNode2->name, std::string("ourWindow"));
                ENSURE_MSG(myXMLNode2->attributes.find("width") != myXMLNode2->attributes.end(), "width should be in attributes of document");
                ENSURE_EQUAL(myXMLNode2->attributes["width"], std::string("300"));
                xmlFreeDoc(doc);
            }

            void perform_loadMultipleXMLTest() {
                xmlDocPtr doc = loadXMLFromMemory(include);
                xmlNode *root_node = xmlDocGetRootElement(doc);
                ENSURE_MSG(root_node,"root node should not be null");
                ENSURE_EQUAL(std::string((const char*)root_node->name), std::string("document"));
                XMLNodePtr myXMLNode(new XMLNode(root_node));
                xmlFreeDoc(doc);

                //ENSURE_EQUAL(std::string((const char*)root_node->name), std::string("document"));  //this is not possible after deleting the document
                ENSURE_MSG(myXMLNode, "XMLNode should not be null");
                ENSURE_EQUAL(myXMLNode->nodeName, std::string("document"));

                doc = loadXMLFromMemory(sparkFile);
                xmlNode *root_node2 = xmlDocGetRootElement(doc);
                ENSURE_MSG(root_node2,"root_node2 should not be null");
                XMLNodePtr myXMLNode2(new XMLNode(root_node2));
                xmlFreeDoc(doc);

                //ENSURE_EQUAL(std::string((const char*)root_node2->name), std::string("Window"));  //why is this possible??? No, it's not possible!

                ENSURE_MSG(myXMLNode2, "XMLNode should not be null");
                ENSURE_EQUAL(myXMLNode2->nodeName, std::string("Window"));
                ENSURE_EQUAL(myXMLNode2->name, std::string("ourWindow"));
                ENSURE_MSG(myXMLNode2->attributes.find("width") != myXMLNode2->attributes.end(), "width should be in attributes of document");
                ENSURE_EQUAL(myXMLNode2->attributes["width"], std::string("300"));

                ENSURE_MSG(root_node,"root node should not be null");
                //ENSURE_EQUAL(std::string((const char*)root_node->name), std::string("document"));  //this is not possible after deleting the document
                ENSURE_MSG(myXMLNode, "XMLNode should not be null");
                ENSURE_EQUAL(myXMLNode->nodeName, std::string("document"));

                //ENSURE_EQUAL(std::string((const char*)root_node2->name), std::string("Window"));  //this is not possible after deleting the document
            }
    };

    const char* XMLNode_UnitTest::include = "<?xml version='1.0'?>\n\
                                   <document xmlns:xi=\"http://www.w3.org/2003/XInclude\">\n\
                                     <p>List of people:</p>\n\
                                     <xi:include href=\"sql:select_name_from_people\"/>\n\
                                   </document>\n";

    const char* XMLNode_UnitTest::sparkFile = "<?xml version=\"1.0\"?>\n\
                                               <Window name=\"ourWindow\" width=\"300\" height=\"300\">\n\
                                                   <Transform name=\"transformB\" x=\"5\" rotationZ=\"-1\">\n\
                                                       <Rectangle name=\"objectA\" x=\"0\" y=\"-5\" width=\"3\" height=\"10\" color=\"[0,0,0.3]\"/>\n\
                                                       <Rectangle name=\"objectB\" width=\"10\" height=\"3\" color=\"[0,0,0.3]\"/>\n\
                                                   </Transform>\n\
                                                   <Transform name=\"transformA\" x=\"-5\" y=\"0\" scaleX=\"2\" >\n\
                                                       <Rectangle name=\"objectA\" width=\"2\" height=\"20\" color=\"[1,0,0.3]\"/>\n\
                                                   </Transform>\n\
                                               </Window>\n";
};
#endif
