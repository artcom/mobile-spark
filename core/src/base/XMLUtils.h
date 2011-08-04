#ifndef XMLUTILS_H
#define XMLUTILS_H

#include <string>

#include <libxml/parser.h>
#include <libxml/tree.h>

namespace cppcore {
    void loadLayout(const std::string & theFilename);
    void printXMLNode(const xmlNode* theNode);
};

#endif // XMLUTILS_H
