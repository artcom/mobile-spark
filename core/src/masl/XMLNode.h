// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_masl_xmlnode_
#define _included_mobile_masl_xmlnode_

#include <string>
#include <sstream>
#include <map>
#include <vector>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <masl/Ptr.h>
#include <masl/string_functions.h>
#include <masl/Exception.h>

#include "numeric_functions.h"

namespace masl {

    DECLARE_EXCEPTION(XMLNodeException, Exception);

    class XMLNode;
    typedef Ptr<XMLNode> XMLNodePtr;
    class XMLNode {
        public:
            XMLNode(const std::string & theXMLString);
            XMLNode(xmlNode* theNode);
            ~XMLNode();

            std::ostream & print(std::ostream & os) const;
            friend inline std::ostream& operator<<(std::ostream& os, const XMLNode& n) {
                return n.print(os);
            }

            template < typename T>
            T getAttributeAs(const std::string & theKey, const T & theDefault) const {
                std::map<std::string, std::string>::const_iterator it = attributes.find(theKey);
                if ( it != attributes.end()) {
                    return masl::as<T>(it->second);
                } else {
                    return theDefault;
                }
            }

            template < typename T>
            T getAttributeAs(const std::string & theKey) const {
                std::map<std::string, std::string>::const_iterator it = attributes.find(theKey);
                if ( it != attributes.end()) {
                    return masl::as<T>(it->second);
                } else {
                    throw XMLNodeException("getAttributeAs<> " + nodeName + std::string(" name='") + name + std::string("' requires attribute ") + theKey, PLUS_FILE_LINE);
                }
            }
        private:
            void init(xmlNode * theNode);
        public:
            std::string nodeName;
            std::string name;
            std::string content;
            std::map<std::string, std::string> attributes;

            //this is only used during initialization and not updated afterwards, use ComponentHierachry instead
            std::vector<XMLNodePtr> children;
    };
};

#endif
