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

        std::string myTitle_I18n = _myXMLNode->getAttributeAs<std::string>("title_I18n","");
        std::string mySubTitle_I18n =_myXMLNode->getAttributeAs<std::string>("subtitle_I18n","");
            
        imageComponent_ = boost::static_pointer_cast<Image>(getChildByName("image"));
        titleComponent_ = boost::static_pointer_cast<Text>(getChildByName("title"));
        subtitleComponent_ = boost::static_pointer_cast<Text>(getChildByName("subtitle"));
        if (src.size() >0 ) {
            imageComponent_->setSrc(src);
        }
         
        if (myTitle_I18n.size() > 0) {
           titleComponent_->setI18nId(myTitle_I18n);
        }
        if (mySubTitle_I18n.size() > 0) {
            subtitleComponent_->setI18nId(mySubTitle_I18n);
        }
    }    
}
