#ifndef _included_mobile_asl_xmlutils_
#define _included_mobile_asl_xmlutils_

#include <string>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "Exception.h"

namespace masl {
    DEFINE_EXCEPTION(XMLParsingException, Exception)

    xmlDocPtr loadXML(const std::string & theFilename);
    xmlDocPtr loadXMLFromMemoryValidate(const std::string & theXMLString);
    xmlDocPtr loadXMLFromMemory(const std::string & theXMLString);
    void printXMLNode(const xmlNode* theNode);
};

#endif
