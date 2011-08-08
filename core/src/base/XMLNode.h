#ifndef XMLNODE_H
#define XMLNODE_H

#include <string>
#include <map>
#include <vector>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <boost/smart_ptr/shared_ptr.hpp>

namespace cppcore {
    class XMLNode {
        public:
            XMLNode(xmlNode* theNode);
            virtual ~XMLNode();
            void print() const;
            void printTree() const;
            float getFloatValue(const std::string & theKey, const float theDefault = 0.0f) const;
            std::vector<float> getFloatArrayValue(const std::string & theKey) const;

            std::string className;
            std::string name;
            std::map<std::string, std::string> attributes;

            const xmlNode* node;
    };

};

#endif // XMLNODE_H
