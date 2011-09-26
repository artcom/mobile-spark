#ifndef _included_mobile_acprojectview_ProjectMenu_
#define _included_mobile_acprojectview_ProjectMenu_

#include <spark/Transform.h>
#include <spark/SparkComponentFactory.h>


namespace acprojectview {

    class ProjectMenu : public spark::Transform {
        public: 
            ProjectMenu(const spark::BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~ProjectMenu();
            virtual void realize();

            void arrangeProjects();
            
            int getPreviewWidth();
            int getPreviewHeight();
            
        private:
            spark::WindowPtr _myWindowPtr;

            int _myVerticalTiling;
            int _myHorizontalTiling;
            int _myGapX;
            int _myGapY;
            
    };
    typedef boost::shared_ptr<ProjectMenu> ProjectMenuPtr;
};

#endif
