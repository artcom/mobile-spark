// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include <animation/AnimationManager.h>
#include <animation/ParallelAnimation.h>
#include <animation/SequenceAnimation.h>
#include <animation/DelayAnimation.h>
#include <animation/Easing.h>

#include <boost/progress.hpp>

#include "spark/Image.h"
#include "ProjectMenu.h"




using namespace spark;

namespace acprojectview {

    ProjectMenu::ProjectMenu(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
        : Transform(theApp, theXMLNode) {
        _myHorizontalTiling = theXMLNode->getAttributeAs<int>("x-tiling",0); 
        _myVerticalTiling = theXMLNode->getAttributeAs<int>("y-tiling",0); 
        _myGapX = theXMLNode->getAttributeAs<int>("x-gap",0); 
        _myGapY = theXMLNode->getAttributeAs<int>("y-gap",0);
        _myCurrentSlide = 0;
          
                   
    }
    
    void
    ProjectMenu::realize() {
        Transform::realize();
        _myWindowPtr = boost::static_pointer_cast<Window>(getRoot());
        _myWidth = _myWindowPtr->getSize()[0];
        _myHeight = _myWindowPtr->getSize()[1];  
        ProjectMenuPtr myPtr = boost::static_pointer_cast<ProjectMenu>(shared_from_this());
        spark::EventCallbackPtr swipeLeftCB = EventCallbackPtr(new ProjectMenuEventCB(myPtr, &ProjectMenu::onSwipeLeftCB));
        _myWindowPtr->addEventListener(GestureEvent::SWIPE_LEFT, swipeLeftCB);
        spark::EventCallbackPtr swipeRightCB = EventCallbackPtr(new ProjectMenuEventCB(myPtr, &ProjectMenu::onSwipeRightCB));
        _myWindowPtr->addEventListener(GestureEvent::SWIPE_RIGHT, swipeRightCB);
        
        spark::EventCallbackPtr myDragCB = EventCallbackPtr(new ProjectMenuEventCB(myPtr, &ProjectMenu::onDrag));
        getRoot()->addEventListener(GestureEvent::PAN, myDragCB);
        spark::EventCallbackPtr myStopDragCB = EventCallbackPtr(new ProjectMenuEventCB(myPtr, &ProjectMenu::onStopDrag));
        getRoot()->addEventListener(TouchEvent::UP, myStopDragCB);
        
        std::string rectNode = "<Rectangle name=\"\" x=\"0\" y=\"0\" width=\"" + masl::as_string(_myWidth) + "\" height=\"" + masl::as_string(_myHeight) + "\" color=\"[0,0,0]\"/>";
        ComponentPtr myCreatedLeft = SparkComponentFactory::get().loadSparkComponentsFromString(getApp(), rectNode); 
        ComponentPtr myCreatedRight = SparkComponentFactory::get().loadSparkComponentsFromString(getApp(), rectNode); 
        _myLeftBlackRectPtr = boost::static_pointer_cast<Rectangle>(myCreatedLeft);
        _myRightBlackRectPtr = boost::static_pointer_cast<Rectangle>(myCreatedRight);
        myPtr->addChild(_myLeftBlackRectPtr);
        myPtr->addChild(_myRightBlackRectPtr);
        
    }

    ProjectMenu::~ProjectMenu() {}

    void
    ProjectMenu::arrangeProjects() {
        _myIsAnimating = false; 
        _myFingerIsStillDown = false;
        _myDragStarted = false;
        ProjectMenuPtr myPtr = boost::static_pointer_cast<ProjectMenu>(shared_from_this());
        
        _myIconWidth = (_myWidth - (_myHorizontalTiling-1)*(_myGapX-1)) / _myHorizontalTiling;
        _myIconHeight = (_myHeight - (_myVerticalTiling-1)*(_myGapY-1)) / _myVerticalTiling;
        const VectorOfComponentPtr & myChildren = myPtr->getChildrenByType(ProjectImpl::SPARK_TYPE);
        _myNumberOfSlides = (myChildren.size()-1)/(_myHorizontalTiling * _myVerticalTiling);
        for (size_t i = 0; i < myChildren.size(); i++) {
            ProjectImplPtr myProject = boost::static_pointer_cast<ProjectImpl>(myChildren[i]);
            ImagePtr image = boost::static_pointer_cast<spark::Image>(myProject->getChildByName("image"));
            TextPtr titlePtr = boost::static_pointer_cast<spark::Text>(myProject->getChildByName("title"));
            TextPtr subtitlePtr = boost::static_pointer_cast<spark::Text>(myProject->getChildByName("subtitle"));
            // set Position:
            int xnr=i/_myVerticalTiling;
            myProject->setX((xnr*_myIconWidth) + _myGapX*(xnr-xnr/_myHorizontalTiling)); 
            myProject->setY((i % _myVerticalTiling)*(_myGapY + _myIconHeight));
        }
        _myLeftBlackRectPtr->setX(-_myWidth);
        _myRightBlackRectPtr->setX((_myNumberOfSlides+1) * _myWidth);
    }
    
    
    void
    ProjectMenu::onSwipeLeftCB(EventPtr theEvent) {
        if (isRendered() && isSensible() && !_myBlockSwiping && _myCurrentSlide < _myNumberOfSlides) {        
            _myFingerIsStillDown = true;
            changeSlide(1);
        }
    }
    
    void
    ProjectMenu::onSwipeRightCB(EventPtr theEvent) {
        if (isRendered() && isSensible() && !_myBlockSwiping && _myCurrentSlide > 0) {        
            _myFingerIsStillDown = true;
            changeSlide(-1);
        }
    }
    
    void 
    ProjectMenu::changeSlide(int dir) {
        ProjectMenuPtr myPtr = boost::static_pointer_cast<ProjectMenu>(shared_from_this());
        if(_myIsAnimating || _myCurrentSlide+dir < 0 || _myCurrentSlide+dir > _myNumberOfSlides) return;
        _myIsAnimating = true;    
        _myCurrentSlide += dir;
        WidgetPropertyAnimationPtr changeAnimation = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(WidgetWeakPtr(WidgetPtr(myPtr)), &Widget::setX, myPtr->getX(), 
                    -_myCurrentSlide*_myWidth, 300,
                    animation::EasingFnc(animation::easeInOutQuad)));
        animation::SequenceAnimationPtr mySeqAnimation = animation::SequenceAnimationPtr(new animation::SequenceAnimation());
        animation::ParallelAnimationPtr myAnimation = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
        myAnimation->add(changeAnimation);
        animation::DelayAnimationPtr myDelayAnim = animation::DelayAnimationPtr(new animation::DelayAnimation(0));
        myDelayAnim->setOnFinish(masl::CallbackPtr(new ProjectMenuCB(myPtr, &ProjectMenu::onDelayFinished)));
        myAnimation->setOnFinish(masl::CallbackPtr(new ProjectMenuCB(myPtr, &ProjectMenu::onAnimationFinished)));
        mySeqAnimation->add(myAnimation);
        mySeqAnimation->add(myDelayAnim);
        animation::AnimationManager::get().play(mySeqAnimation);
    }
    
    void ProjectMenu::onDelayFinished() {        
        _myIsAnimating = false;
    }
    
    void ProjectMenu::onAnimationFinished() {      
        ProjectMenuPtr myPtr = boost::static_pointer_cast<ProjectMenu>(shared_from_this());
        _myXBeforeDrag = myPtr->getX();
    }

    
    void ProjectMenu::onDrag(spark::EventPtr theEvent) {
        if(_myIsAnimating || _myFingerIsStillDown || !isSensible() ) return;
        ProjectMenuPtr myPtr = boost::static_pointer_cast<ProjectMenu>(shared_from_this());
        int myPosX = myPtr->getX();
        if(!_myDragStarted) {
            _myXBeforeDrag = myPosX;
        }
        if (_myDragStarted) _myBlockSwiping=true;
        _myDragStarted=true;
        GestureEventPtr myEvent = boost::static_pointer_cast<GestureEvent>(theEvent);
        int dx = myEvent->getTranslateX();
        myPtr->setX(_myXBeforeDrag+dx);
    }
    
    void ProjectMenu::onStopDrag(spark::EventPtr theEvent) {
        _myDragStarted = false;
        _myFingerIsStillDown = false;
        _myBlockSwiping=false;
        ProjectMenuPtr myPtr = boost::static_pointer_cast<ProjectMenu>(shared_from_this());
        int myPosX = myPtr->getX();
        if(myPosX - _myXBeforeDrag > _myWidth/4 && _myCurrentSlide > 0) {
            _myDragStarted = false;
//            _myFingerIsStillDown = true;
            changeSlide(-1);
            return;
        } else if (myPosX - _myXBeforeDrag < -_myWidth/4 && _myCurrentSlide < _myNumberOfSlides){
            _myDragStarted = false;
//            _myFingerIsStillDown = true;
            changeSlide(1);
            return;
        }

        changeSlide(0);
    }

    
}
