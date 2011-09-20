#ifndef _included_mobile_acprojectview_ProjectMenu_
#define _included_mobile_acprojectview_ProjectMenu_

#include <spark/Transform.h>

namespace acprojectview {

    class ProjectMenu : public spark::Transform {
        public: 
            ProjectMenu(const spark::BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode, spark::ComponentPtr theParent);
            virtual ~ProjectMenu();
            void arrangeProjects(const int theWidth, const int theHeight);
            
            int getPreviewWidth();
            int getPreviewHeight();
            
        private:
            
    };
    typedef boost::shared_ptr<ProjectMenu> ProjectMenuPtr;
};

#endif
