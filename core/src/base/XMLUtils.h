#ifndef _included_mobile_asl_xmlutils_
#define _included_mobile_asl_xmlutils_

#include <string>

#include <libxml/parser.h>
#include <libxml/tree.h>

namespace asl {
    xmlNode* loadXML(const std::string & theFilename);
    void printXMLNode(const xmlNode* theNode);
};

#endif
