#ifndef _included_mobile_acprojectview_ProjectMenu_
#define _included_mobile_acprojectview_ProjectMenu_

#include <spark/Transform.h>
#include <spark/SparkComponentFactory.h>
#include <spark/Window.h>



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
            void onDragCB(spark::EventPtr theEvent);
            void onFingerDownCB(spark::EventPtr theEvent);

            
            spark::WindowPtr _myWindowPtr;
            int _myVerticalTiling;
            int _myHorizontalTiling;
            int _myGapX;
            int _myGapY;
            int _myStartDragX;
            
    };
    typedef boost::shared_ptr<ProjectMenu> ProjectMenuPtr;
    typedef spark::MemberFunctionEventCallback<ProjectMenu, ProjectMenuPtr>ProjectMenuEventCB;

};

#endif
