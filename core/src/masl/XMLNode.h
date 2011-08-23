#ifndef _included_mobile_asl_xmlnode_
#define _included_mobile_asl_xmlnode_

#include <string>
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
            void printTree() const;
            float getFloatValue(const std::string & theKey, const float theDefault = 0.0f) const;
            bool getBoolValue(const std::string & theKey, const bool theDefault = true) const;
            const std::string & getStringValue(const std::string & theKey, const std::string theDefault = "") const;
                                
            std::vector<float> getFloatArrayValue(const std::string & theKey) const;
            vector4 getVector4Value(const std::string & theKey, const vector4 theDefault = vector4(0,0,0,0)) const;

            std::string nodeName;
            std::string name;
            std::map<std::string, std::string> attributes;

            const xmlNode* node;
    };

    typedef boost::shared_ptr<XMLNode> XMLNodePtr;
};

#endif
