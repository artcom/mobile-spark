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
        ProjectMenuPtr myPtr = boost::static_pointer_cast<ProjectMenu>(shared_from_this());
        spark::EventCallbackPtr swipeLeftCB = EventCallbackPtr(new ProjectMenuEventCB(myPtr, &ProjectMenu::onSwipeLeftCB));
        _myWindowPtr->addEventListener(GestureEvent::SWIPE_LEFT, swipeLeftCB);
        spark::EventCallbackPtr swipeRightCB = EventCallbackPtr(new ProjectMenuEventCB(myPtr, &ProjectMenu::onSwipeRightCB));
        _myWindowPtr->addEventListener(GestureEvent::SWIPE_RIGHT, swipeRightCB);
    }

    ProjectMenu::~ProjectMenu() {}

    void
    ProjectMenu::arrangeProjects() {
        _myIsAnimating = false; 
        ProjectMenuPtr myPtr = boost::static_pointer_cast<ProjectMenu>(shared_from_this());
        _myWidth = _myWindowPtr->getSize()[0];
        _myHeight = _myWindowPtr->getSize()[1];
        _myIconWidth = (_myWidth - (_myHorizontalTiling-1)*_myGapY) / _myHorizontalTiling;
        _myIconHeight = (_myHeight - (_myVerticalTiling-1)*_myGapX) / _myVerticalTiling;
        const VectorOfComponentPtr & myChildren = myPtr->getChildrenByType(ProjectImpl::SPARK_TYPE);
        _myNumberOfSlides = (myChildren.size()-1)/(_myHorizontalTiling * _myVerticalTiling);
        boost::timer::timer myTimer;

        for (size_t i = 0; i < myChildren.size(); i++) {
            ProjectImplPtr myProject = boost::static_pointer_cast<ProjectImpl>(myChildren[i]);
            ImagePtr image = boost::static_pointer_cast<spark::Image>(myProject->getChildByName("image"));
            TextPtr titlePtr = boost::static_pointer_cast<spark::Text>(myProject->getChildByName("title"));
            TextPtr subtitlePtr = boost::static_pointer_cast<spark::Text>(myProject->getChildByName("subtitle"));
            // set Position:
            myProject->setX((i/_myVerticalTiling)*(_myGapX + _myIconWidth)); 
            myProject->setY((i % _myVerticalTiling)*(_myGapY + _myIconHeight));
        }
         AC_PRINT << "******************************************************* " << myTimer.elapsed();
    }
    
    
    void
    ProjectMenu::onSwipeLeftCB(EventPtr theEvent) {
        if (isRendered()) {        
            AC_PRINT << "Swipe menu";
            changeSlide(1);
        }
    }
    
    void
    ProjectMenu::onSwipeRightCB(EventPtr theEvent) {
        if (isRendered()) {        
            AC_PRINT << "Swipe menu";
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
                new WidgetPropertyAnimation(myPtr, &Widget::setX, myPtr->getX(), 
                    myPtr->getX()-(_myWidth+_myGapX*(_myHorizontalTiling-1))*dir, 300,
                    animation::EasingFnc(animation::easeInOutQuad)));
         
        animation::SequenceAnimationPtr mySeqAnimation = animation::SequenceAnimationPtr(new animation::SequenceAnimation());
        animation::ParallelAnimationPtr myAnimation = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
        myAnimation->add(changeAnimation);
        animation::DelayAnimationPtr myDelayAnim = animation::DelayAnimationPtr(new animation::DelayAnimation(0));
        myDelayAnim->setOnFinish(masl::CallbackPtr(
                        new masl::MemberFunctionCallback<ProjectMenu, ProjectMenuPtr>(myPtr, &ProjectMenu::onDelayFinished)));
            
        mySeqAnimation->add(myAnimation);
        mySeqAnimation->add(myDelayAnim);
        animation::AnimationManager::get().play(mySeqAnimation);
    }
    
    void ProjectMenu::onDelayFinished() {        
        _myIsAnimating = false;
    }

    
}
