#include "ProjectImpl.h"
#include <spark/SparkComponentFactory.h>

using namespace spark;

namespace acprojectview {
        
    const char * const ProjectImpl::SPARK_TYPE = "ProjectImpl";

    
    ProjectImpl::ProjectImpl(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
        : Transform(theApp, theXMLNode) {
    }

    ProjectImpl::~ProjectImpl() {}

    void ProjectImpl::realize() {
        Transform::realize();
        std::string src = _myXMLNode->getAttributeAs<std::string>("src",""); 
        std::string title = _myXMLNode->getAttributeAs<std::string>("title",""); 
        std::string subtitle = _myXMLNode->getAttributeAs<std::string>("subtitle",""); 
        imageComponent_ = boost::static_pointer_cast<Image>(getChildByName("image"));
        titleComponent_ = boost::static_pointer_cast<Text>(getChildByName("title"));
        subtitleComponent_ = boost::static_pointer_cast<Text>(getChildByName("subtitle"));
         
        if (src.size() >0 ) {
            imageComponent_->setSrc(src);
        }
         
        if (title.size() > 0) {
           titleComponent_->setText(title);
        }
        if (subtitle.size() > 0) {
            subtitleComponent_->setText(subtitle);
        }
    }    
}
