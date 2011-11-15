// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_acprojectview_ProjectMenu_
#define _included_mobile_acprojectview_ProjectMenu_

#include <spark/Transform.h>
#include <spark/SparkComponentFactory.h>
#include <spark/Window.h>
#include <spark/Rectangle.h>
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
            void onAnimationFinished();       
            void onDrag(spark::EventPtr theEvent);
            void onStopDrag(spark::EventPtr theEvent);

            spark::WindowPtr _myWindowPtr;
            spark::RectanglePtr _myLeftBlackRectPtr;
            spark::RectanglePtr _myRightBlackRectPtr;
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
            int _myXBeforeDrag;
            bool _myIsAnimating; 
            bool _myDragStarted;
            bool _myFingerIsStillDown;
            bool _myBlockSwiping;


            
    };
    typedef masl::Ptr<ProjectMenu> ProjectMenuPtr;
    typedef masl::WeakPtr<ProjectMenu> ProjectMenuWeakPtr;
    typedef masl::MemberFunctionCallback<ProjectMenu, ProjectMenuWeakPtr> ProjectMenuCB;
    typedef spark::MemberFunctionEventCallback<ProjectMenu, ProjectMenuWeakPtr> ProjectMenuEventCB;
};

#endif
