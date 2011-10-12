#ifndef _included_mobile_masl_xmlutils_
#define _included_mobile_masl_xmlutils_

#include <string>
#include <vector>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "Exception.h"
#include "XMLNode.h"

namespace masl {
    DECLARE_EXCEPTION(XMLParsingException, Exception)

    xmlDocPtr loadXML(const std::string & theFilename);
    xmlDocPtr loadXMLFromMemoryValidate(const std::string & theXMLString);
    xmlDocPtr loadXMLFromMemory(const std::string & theXMLString);
    void printXMLNode(const xmlNode* theNode);
    std::vector<XMLNodePtr> loadXMLFromMemoryWithXPath(const std::string & theXMLString, const std::string & theXPath);
};

#endif
