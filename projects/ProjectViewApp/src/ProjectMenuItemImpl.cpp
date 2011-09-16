#include "ProjectMenuItemImpl.h"
#include <spark/SparkComponentFactory.h>

using namespace spark;

namespace projectviewapp {

    //needed for component factory
    namespace  {
        const bool registered = spark::SparkComponentFactory::get().registerComponent("ProjectMenuItemImpl", spark::create<ProjectMenuItemImpl>);
    }
    
    ProjectMenuItemImpl::ProjectMenuItemImpl(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent)
        : Transform(theApp, theXMLNode, theParent) {
    }

    ProjectMenuItemImpl::~ProjectMenuItemImpl() {}

    void ProjectMenuItemImpl::realize() {
         AC_PRINT << "##################################### PROJECTMENUItem KONSTRUKTOR";
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
