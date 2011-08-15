#ifndef _included_mobile_asl_xmlutils_
#define _included_mobile_asl_xmlutils_

#include <string>

#include <libxml/parser.h>
#include <libxml/tree.h>

namespace masl {
    xmlDocPtr loadXML(const std::string & theFilename);
    xmlDocPtr loadXMLFromMemory(const std::string & theXMLString);
    void printXMLNode(const xmlNode* theNode);
};

#endif
