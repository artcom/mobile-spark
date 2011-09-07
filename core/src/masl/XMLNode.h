#ifndef _included_mobile_asl_xmlnode_
#define _included_mobile_asl_xmlnode_

#include <string>
#include <sstream>
#include <map>
#include <vector>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <masl/string_functions.h>

#include "numeric_functions.h"

namespace masl {

    DEFINE_EXCEPTION(XMLNodeException, Exception);

    class XMLNode {
        public:
            XMLNode(xmlNode* theNode);
            virtual ~XMLNode();
            void print() const;
            std::ostream & print(std::ostream & os) const;
            void printTree() const;

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

            std::string nodeName;
            std::string name;
            std::map<std::string, std::string> attributes;

            const xmlNode* node;

    };

    typedef boost::shared_ptr<XMLNode> XMLNodePtr;
};

#endif
