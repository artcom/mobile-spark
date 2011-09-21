#include "ContentImage.h"
#include <spark/SparkComponentFactory.h>

using namespace spark;

namespace acprojectview {

    const char * const ContentImage::SPARK_TYPE = "ContentImage";
    
        
    ContentImage::ContentImage(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent)
        : Container(theApp, theXMLNode, theParent) {
        _mySrc = _myXMLNode->getAttributeAs<std::string>("src",""); 
    }

    ContentImage::~ContentImage() {}

    std::string ContentImage::getSrc() {return _mySrc;}
}
