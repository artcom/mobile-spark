#include "Description.h"
#include <spark/SparkComponentFactory.h>

using namespace spark;

namespace acprojectview {

    const char * const Description::SPARK_TYPE = "Description";
    
        
    Description::Description(const BaseAppPtr theApp, const XMLNodePtr theXMLNode)
        : Container(theApp, theXMLNode) {
        _myText = _myXMLNode->getAttributeAs<std::string>("text","");
    }

    Description::~Description() {}

    std::string Description::getText() const{
        return _myText;
    }
}
