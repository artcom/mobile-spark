// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

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
