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

        _myWindowPtr = boost::static_pointer_cast<Window>(getRoot());                
    }

    void ProjectViewerImpl::showProject(ProjectImplPtr currentProject) {
         _myIsAnimating = false;            
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
         //AC_PRINT << "__sadf_______________________" << _myDescription->getTextSize()[1];
         image0->setX(0);
         image1->setX(_myWidth);
         image2->setX(-_myWidth);
         _myCurrentImage = 0;
         _myDisplayedImage = 0;
         
         image0->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[0])->getSrc());
        AC_PRINT << "ProjectViewerImpl::showProject start";
            
         autoScaleImage(image0);
            AC_PRINT << "ProjectViewerImpl::showProject end";
         
         image0->setVisible(true);
         image1->setVisible(false);
         image2->setVisible(false);
         setVisible(false);
    }
    
    void ProjectViewerImpl::autoScaleImage(ImagePtr theImage) {
        AC_PRINT << "ProjectViewerImpl::autoScaleImage";
        int myImageWidth = theImage->getTextureSize()[0];
        int myImageHeight = theImage->getTextureSize()[1];
        float myImageRatio = float(myImageWidth)/myImageHeight;
        
        int myWindowWidth = _myWindowPtr->getSize()[0];
        int myWindowHeight = _myWindowPtr->getSize()[1];
        float myWindowRatio = float(myWindowWidth)/myWindowHeight;
        
        AC_PRINT << "image : " << theImage->getName() << " size: " << myImageWidth << "/" << myImageHeight << " ratio: " << myImageRatio;
        AC_PRINT << "window : " << _myWindowPtr->getName() << " size: " << myWindowWidth << "/" << myWindowHeight << " ratio: " << myWindowRatio;

        int myNewWidth = 0;
        int myNewHeight = 0;
        if (myImageRatio == myWindowRatio) {
            myNewWidth = myWindowWidth;
            myNewHeight = myWindowHeight;
        } else {
            float myWidthScale = float(myImageWidth) / myWindowWidth;
            float myHeightScale = float(myImageWidth) / myWindowWidth;
            if (myWidthScale > myHeightScale) {
                // with adapting width we get a bigger size    
                myNewWidth = myWidthScale * myWindowWidth;   
                myNewHeight = myNewWidth / myImageRatio;
            } else {
                myNewHeight = myHeightScale * myWindowHeight;
                myNewWidth = myNewHeight / myImageRatio;
            }
        }
        theImage->setScaleX(float(myNewWidth)/myImageWidth);
        theImage->setScaleY(float(myNewHeight)/myImageHeight);
        AC_PRINT << "image will be scaled to  : " << myNewWidth << "/" << myNewHeight;
    }
    
    void ProjectViewerImpl::initiateClose() {
        image0->setVisible(false);
        image1->setVisible(false);
        image2->setVisible(false);
        if (_myCurrentImage == 0) {
            image0->setVisible(true);
        } else if (_myCurrentImage == 1) {
            image1->setVisible(true);
        } else if (_myCurrentImage == 2) {
            image2->setVisible(true);
        }
    }
    void ProjectViewerImpl::loadInitialSet() {
         if (_myNumberOfImages > 0) {
            AC_PRINT << "ProjectViewerImpl::loadInitialSet start";
             image1->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[1%_myNumberOfImages])->getSrc());
             autoScaleImage(image1);

             image2->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[_myNumberOfImages-1])->getSrc());
             autoScaleImage(image2);
            AC_PRINT << "ProjectViewerImpl::loadInitialSet end";
        }        
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
                    animation::EasingFnc(animation::easeInOutQuad)));
        WidgetPropertyAnimationPtr changeAnimation1 = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(image1, &Widget::setX, image1->getX(), image1->getX()-_myWidth*dir, 300,
                    animation::EasingFnc(animation::easeInOutQuad)));
        WidgetPropertyAnimationPtr changeAnimation2 = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(image2, &Widget::setX, image2->getX(), image2->getX()-_myWidth*dir, 300,
                    animation::EasingFnc(animation::easeInOutQuad)));
        
        animation::SequenceAnimationPtr mySeqAnimation = animation::SequenceAnimationPtr(new animation::SequenceAnimation());
        animation::ParallelAnimationPtr myAnimation = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
        ProjectViewerImplPtr ptr = boost::static_pointer_cast<ProjectViewerImpl>(shared_from_this());
        myAnimation->setOnFinish(masl::CallbackPtr(
                        new masl::MemberFunctionCallback<ProjectViewerImpl, ProjectViewerImplPtr>(ptr, &ProjectViewerImpl::onAnimationFinished)));
        myAnimation->add(changeAnimation0);
        myAnimation->add(changeAnimation1);
        myAnimation->add(changeAnimation2);

        animation::DelayAnimationPtr myReadNextImagesAnim = animation::DelayAnimationPtr(new animation::DelayAnimation(0));
        myReadNextImagesAnim->setOnFinish(masl::CallbackPtr(
                        new masl::MemberFunctionCallback<ProjectViewerImpl, ProjectViewerImplPtr>(ptr, &ProjectViewerImpl::onLoadNextImages)));
            
        mySeqAnimation->add(myAnimation);
        mySeqAnimation->add(myReadNextImagesAnim);
        animation::AnimationManager::get().play(mySeqAnimation);
    }
    
    void ProjectViewerImpl::onLoadNextImages() {
            AC_PRINT << "ProjectViewerImpl::onLoadNextImages start";

        if ((_myCurrentImage-_myDirection+3)%3 == 0) {
            image0->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[(_myCurrentImage+_myDirection*2+_myNumberOfImages) %_myNumberOfImages])->getSrc());
            autoScaleImage(image0);
        } else if ((_myCurrentImage-_myDirection+3)%3 == 1) {
            image1->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[(_myCurrentImage+_myDirection*2+_myNumberOfImages) %_myNumberOfImages])->getSrc());
            autoScaleImage(image1);
        } else if ((_myCurrentImage-_myDirection+3)%3 == 2) {
            image2->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[(_myCurrentImage+_myDirection*2+_myNumberOfImages) %_myNumberOfImages])->getSrc());
            autoScaleImage(image2);
        }
            AC_PRINT << "ProjectViewerImpl::onLoadNextImages end";
        _myCurrentImage = (_myCurrentImage + _myDirection + _myNumberOfImages) % _myNumberOfImages;
        _myIsAnimating = false;            
        
    }
    void ProjectViewerImpl::onAnimationFinished() {
        if ((_myCurrentImage-_myDirection+3)%3 == 0) {
            image0->setX((image0->getX() + _myWidth*_myDirection) * -1);
        } else if ((_myCurrentImage-_myDirection+3)%3 == 1) {
            image1->setX((image1->getX() + _myWidth*_myDirection) * -1);
        } else if ((_myCurrentImage-_myDirection+3)%3 == 2) {
            image2->setX((image2->getX() + _myWidth*_myDirection) * -1);
        }
    }

        
}
