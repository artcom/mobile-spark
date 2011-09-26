#include "ProjectViewerImpl.h"
#include <spark/SparkComponentFactory.h>
#include <animation/AnimationManager.h>
#include <animation/ParallelAnimation.h>
#include <animation/SequenceAnimation.h>
#include <animation/DelayAnimation.h>
#include <animation/Easing.h>



using namespace spark;

namespace acprojectview {
        
    const char * const ProjectViewerImpl::SPARK_TYPE = "ProjectViewerImpl";

    
    ProjectViewerImpl::ProjectViewerImpl(const BaseAppPtr theApp, const XMLNodePtr theXMLNode)
        : Transform(theApp, theXMLNode) {
        std::string image = _myXMLNode->getAttributeAs<std::string>("im",""); 
        _myWidth = 800;
        _myHeight = 480;
        image0 = boost::static_pointer_cast<Image>(getChildByName("image0"));
        image1 = boost::static_pointer_cast<Image>(getChildByName("image1"));
        image2 = boost::static_pointer_cast<Image>(getChildByName("image2"));
        _myDescription = boost::static_pointer_cast<Text>(getChildByName("description"));
        _myDescription->setText("");
        
    }

    ProjectViewerImpl::~ProjectViewerImpl() {}

    void
    ProjectViewerImpl::realize() {
        Transform::realize();
        ProjectViewerImplPtr ptr = boost::static_pointer_cast<ProjectViewerImpl>(shared_from_this());

        spark::EventCallbackPtr mySwipeCB = EventCallbackPtr(new ProjectViewerImplCB(ptr, &ProjectViewerImpl::onSwipe));
        getRoot()->addEventListener(GestureEvent::SWIPE_LEFT, mySwipeCB);
        getRoot()->addEventListener(GestureEvent::SWIPE_RIGHT, mySwipeCB);

    }

    void ProjectViewerImpl::showProject(ProjectImplPtr currentProject) {
         _myIsAnimating = false;     
         boost::static_pointer_cast<Widget>(getChildByName("textbackground"))->setAlpha(0.5);
         _myCurrentProject = currentProject;
         _myContentImages = _myCurrentProject->getChildrenByType(ContentImage::SPARK_TYPE);
         _myNumberOfImages = _myContentImages.size();
         if (_myNumberOfImages < 1) {
             image0->setVisible(false);
             image1->setVisible(false);
             image2->setVisible(false);
             return;
         }
         image0->setVisible(true);
         DescriptionPtr txt = boost::static_pointer_cast<Description>(_myCurrentProject->getChildByName("description"));
         if (txt == 0) {
             _myDescription->setText("");
         } else {
             _myDescription->setText(txt->getText());
         }
         //AC_PRINT << "__sadf_______________________" << _myDescription->getTextSize();
         image0->setX(0);
         image1->setX(_myWidth);
         image2->setX(-_myWidth);
         _myCurrentImage = 0;
         _myDisplayedImage = 0;
         image0->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[0])->getSrc());
         image1->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[1%_myNumberOfImages])->getSrc());
         image2->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[_myNumberOfImages-1])->getSrc());

    }
    
    void ProjectViewerImpl::onSwipe(EventPtr theEvent) {
    	changeImage(theEvent->getType() == "swipe-right" ? -1 :  +1);
    }
    
    void ProjectViewerImpl::changeImage(int dir) {
        if(_myIsAnimating) return;
        _myIsAnimating = true;      
        image1->setVisible(true);
        image2->setVisible(true);
        _myDirection =dir;
        WidgetPropertyAnimationPtr changeAnimation0 = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(image0, &Widget::setX, image0->getX(), image0->getX()-_myWidth*dir, 300,
                    animation::EasingFnc(animation::easeInOutSine)));
        WidgetPropertyAnimationPtr changeAnimation1 = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(image1, &Widget::setX, image1->getX(), image1->getX()-_myWidth*dir, 300,
                    animation::EasingFnc(animation::easeInOutSine)));
        WidgetPropertyAnimationPtr changeAnimation2 = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(image2, &Widget::setX, image2->getX(), image2->getX()-_myWidth*dir, 300,
                    animation::EasingFnc(animation::easeInOutSine)));
        
        animation::ParallelAnimationPtr myAnimation = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
        ProjectViewerImplPtr ptr = boost::static_pointer_cast<ProjectViewerImpl>(shared_from_this());
        myAnimation->setOnFinish(masl::CallbackPtr(
                        new masl::MemberFunctionCallback<ProjectViewerImpl, ProjectViewerImplPtr>(ptr, &ProjectViewerImpl::onAnimationFinished)));
        myAnimation->add(changeAnimation0);
        myAnimation->add(changeAnimation1);
        myAnimation->add(changeAnimation2);


        animation::AnimationManager::get().play(myAnimation);
    }
    
    void ProjectViewerImpl::onAnimationFinished() {
        if ((_myCurrentImage-_myDirection+3)%3 == 0) {
            image0->setX((image0->getX() + _myWidth*_myDirection) * -1);
            image0->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[(_myCurrentImage+_myDirection*2+_myNumberOfImages) %_myNumberOfImages])->getSrc());
        } else if ((_myCurrentImage-_myDirection+3)%3 == 1) {
            image1->setX((image1->getX() + _myWidth*_myDirection) * -1);
            image1->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[(_myCurrentImage+_myDirection*2+_myNumberOfImages) %_myNumberOfImages])->getSrc());
        } else if ((_myCurrentImage-_myDirection+3)%3 == 2) {
            image2->setX((image2->getX() + _myWidth*_myDirection) * -1);
            image2->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[(_myCurrentImage+_myDirection*2+_myNumberOfImages) %_myNumberOfImages])->getSrc());
        }
        _myCurrentImage = (_myCurrentImage + _myDirection + _myNumberOfImages) % _myNumberOfImages;
        _myIsAnimating = false;            
    }

        
}
