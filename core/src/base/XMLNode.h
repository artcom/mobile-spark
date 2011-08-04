#ifndef XMLNODE_H
#define XMLNODE_H

#include <string>
#include <map>
#include <vector>

#include <libxml/parser.h>
#include <libxml/tree.h>

namespace cppcore {
    class XMLNode {
        public:
            XMLNode(xmlNode* theNode);
            virtual ~XMLNode();
            void print() const;
            void printTree() const;
            float getFloatValue(const std::string & theKey, const float theDefault = 0.0f) const;

            std::string name;
            std::map<std::string, std::string> attributes;
            std::vector<XMLNode*> children;


        private:
            const xmlNode* _myNode;
    };
};

#endif // XMLNODE_H
