#ifndef _included_mobile_asl_xmlnode_
#define _included_mobile_asl_xmlnode_

#include <string>
#include <sstream>
#include <map>
#include <vector>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "numeric_functions.h"

namespace masl {
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
            
            std::string getStringValue(const std::string & theKey, const std::string & theDefault = "") const;
            float getFloatValue(const std::string & theKey, const float theDefault = 0.0f) const;
            bool getBoolValue(const std::string & theKey, const bool theDefault = true) const;
                                
            std::vector<float> getFloatArrayValue(const std::string & theKey) const;
            vector4 getVector4Value(const std::string & theKey, const vector4 theDefault = vector4(0,0,0,0)) const;
            vector2 getVector2Value(const std::string & theKey, const vector2 theDefault = vector2(0,0)) const;

            std::string nodeName;
            std::string name;
            std::map<std::string, std::string> attributes;

            const xmlNode* node;

    };

    typedef boost::shared_ptr<XMLNode> XMLNodePtr;
};

#endif
