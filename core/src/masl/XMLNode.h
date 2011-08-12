#ifndef _included_mobile_asl_xmlnode_
#define _included_mobile_asl_xmlnode_

#include <string>
#include <map>
#include <vector>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <boost/smart_ptr/shared_ptr.hpp>

namespace masl {
    class XMLNode {
        public:
            XMLNode(xmlNode* theNode);
            virtual ~XMLNode();
            void print() const;
            void printTree() const;
            float getFloatValue(const std::string & theKey, const float theDefault = 0.0f) const;
            std::vector<float> getFloatArrayValue(const std::string & theKey) const;

            std::string nodeName;
            std::string name;
            std::map<std::string, std::string> attributes;

            const xmlNode* node;
    };

    typedef boost::shared_ptr<XMLNode> XMLNodePtr;
};

#endif
