#ifndef _included_mobile_acprojectview_ProjectImpl_
#define _included_mobile_acprojectview_ProjectImpl_

#include <spark/Transform.h>
#include <spark/Image.h>
#include <spark/Text.h>


namespace acprojectview {

    class ProjectImpl : public spark::Transform {
        public: 
            ProjectImpl(const spark::BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode, spark::ComponentPtr theParent);
            virtual ~ProjectImpl();
            virtual void realize();
            
            static const char * const SPARK_TYPE;             
            virtual const char * const & getType() const { return ProjectImpl::SPARK_TYPE;};
        private:
            spark::ImagePtr imageComponent_; 
            spark::TextPtr titleComponent_;
            spark::TextPtr subtitleComponent_;
    };
    
    typedef boost::shared_ptr<ProjectImpl> ProjectImplPtr;
};

#endif
