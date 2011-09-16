#ifndef _included_mobile_acprojectview_ProjectMenuItemImpl_
#define _included_mobile_acprojectview_ProjectMenuItemImpl_

#include <spark/Transform.h>
#include <spark/Image.h>
#include <spark/Text.h>


namespace acprojectview {

    class ProjectMenuItemImpl : public spark::Transform {
        public: 
            ProjectMenuItemImpl(const spark::BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode, spark::ComponentPtr theParent);
            virtual ~ProjectMenuItemImpl();
            virtual void realize();
        private:
            spark::ImagePtr imageComponent_; 
            spark::TextPtr titleComponent_;
            spark::TextPtr subtitleComponent_;
    };
    
    typedef boost::shared_ptr<ProjectMenuItemImpl> ProjectMenuItemImplPtr;
};

#endif
