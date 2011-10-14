#ifndef _included_mobile_acprojectview_ProjectMenu_
#define _included_mobile_acprojectview_ProjectMenu_

#include <spark/Transform.h>
#include <spark/SparkComponentFactory.h>
#include <spark/Window.h>
#include "ProjectImpl.h"



namespace acprojectview {

    class ProjectMenu : public spark::Transform {
        public: 
            ProjectMenu(const spark::BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~ProjectMenu();
            virtual void realize();

            void arrangeProjects();
            
            int getIconWidth() {return _myIconWidth;}
            int getIconHeight() {return _myIconHeight;}
            int getHorizontalTiling() { return _myHorizontalTiling;}
            int getVerticalTiling() { return _myVerticalTiling;}
            int getGapX() {return _myGapX;}
            int getGapY() {return _myGapY;}
            int getCurrentSlide() {return _myCurrentSlide;};
        private:
            void onSwipeLeftCB(spark::EventPtr theEvent);
            void onSwipeRightCB(spark::EventPtr theEvent);
            void changeSlide(int dir);
            void onDelayFinished();


            spark::WindowPtr _myWindowPtr;
            int _myVerticalTiling;
            int _myHorizontalTiling;
            int _myIconWidth;
            int _myIconHeight;
            int _myGapX;
            int _myGapY;
            int _myWidth;
            int _myHeight;
            int _myNumberOfSlides;
            int _myCurrentSlide;
            bool _myIsAnimating;      

           
            
    };
    typedef masl::Ptr<ProjectMenu> ProjectMenuPtr;
    typedef masl::WeakPtr<ProjectMenu> ProjectMenuWeakPtr;
    typedef spark::MemberFunctionEventCallback<ProjectMenu, ProjectMenuWeakPtr>ProjectMenuEventCB;

};

#endif
