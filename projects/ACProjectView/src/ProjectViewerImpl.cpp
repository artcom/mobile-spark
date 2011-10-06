#include "ProjectViewerImpl.h"
#include <spark/Window.h>
#include <spark/Rectangle.h>
#include <spark/SparkComponentFactory.h>
#include <animation/AnimationManager.h>
#include <animation/ParallelAnimation.h>
#include <animation/SequenceAnimation.h>
#include <animation/DelayAnimation.h>
#include <animation/Easing.h>



using namespace spark;
using namespace std;

namespace acprojectview {
        
    const char * const ProjectViewerImpl::SPARK_TYPE = "ProjectViewerImpl";

    
    ProjectViewerImpl::ProjectViewerImpl(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
        : Transform(theApp, theXMLNode) {
        _imageTransform0 = boost::static_pointer_cast<Transform>(getChildByName("image_0_transform"));
        _imageTransform1 = boost::static_pointer_cast<Transform>(getChildByName("image_1_transform"));
        _imageTransform2 = boost::static_pointer_cast<Transform>(getChildByName("image_2_transform"));
        _image0 = boost::static_pointer_cast<Image>(_imageTransform0->getChildByName("image"));
        _image1 = boost::static_pointer_cast<Image>(_imageTransform1->getChildByName("image"));
        _image2 = boost::static_pointer_cast<Image>(_imageTransform2->getChildByName("image"));
        _myPopup = boost::static_pointer_cast<Transform>(getChildByName("popup"));
        _myPopupBG = boost::static_pointer_cast<Rectangle>(_myPopup->getChildByName("popup_bg"));
                    
        _myDescription = boost::static_pointer_cast<Text>(_myPopup->getChildByName("description"));
        _myPopUpTitle = boost::static_pointer_cast<Text>(_myPopup->getChildByName("title"));
        _myPopUpSubTitle = boost::static_pointer_cast<Text>(_myPopup->getChildByName("subtitle"));
        _myPopUpPfeil = boost::static_pointer_cast<Image>(_myPopup->getChildByName("pfeil"));

        _myDescription->setText("");   
        _myPopUpTitle->setText("");   
        _myPopUpSubTitle->setText("");   
        
    }

    ProjectViewerImpl::~ProjectViewerImpl() {}

    void
    ProjectViewerImpl::realize() {
        Transform::realize();
        ProjectViewerImplPtr ptr = boost::static_pointer_cast<ProjectViewerImpl>(shared_from_this());
        spark::EventCallbackPtr mySwipeCB = EventCallbackPtr(new ProjectViewerImplCB(ptr, &ProjectViewerImpl::onSwipe));
        getRoot()->addEventListener(GestureEvent::SWIPE_LEFT, mySwipeCB);
        getRoot()->addEventListener(GestureEvent::SWIPE_RIGHT, mySwipeCB);

    
        spark::EventCallbackPtr mySwipeUpDownCB = EventCallbackPtr(new ProjectViewerImplCB(ptr, &ProjectViewerImpl::onOpenClosePopup));
        _myPopupBG->addEventListener(TouchEvent::PICKED, mySwipeUpDownCB);
            
        _myWindowPtr = boost::static_pointer_cast<Window>(getRoot());                
        _myWidth = _myWindowPtr->getSize()[0];
        _myHeight = _myWindowPtr->getSize()[1];

        boost::static_pointer_cast<Rectangle>(getChildByName("background"))->setSize(vector2(_myWidth,_myHeight));
        _myDescription->setMaxWidth(_myWidth - (2*_myDescription->getX()));
        _myDescription->setMaxWidth( (_myWidth/2.0) - _myDescription->getX() - _myDescription->getX()/2.0);
        
        _myPopUpTitle->setMaxWidth(_myWidth - (2*_myPopUpTitle->getX()));
        _myPopUpSubTitle->setMaxWidth(_myWidth - (2*_myPopUpTitle->getX()));
        
        _myPopupBG->getShape()->setDimensions(_myWidth, 0);
            
    }

    void ProjectViewerImpl::showProject(ProjectImplPtr currentProject) {
        string myDescription_I18n = currentProject->getNode()->getAttributeAs<std::string>("description_I18n","");
        string myTitle_I18n = currentProject->getNode()->getAttributeAs<std::string>("title_I18n","");
        string mySubTitle_I18n = currentProject->getNode()->getAttributeAs<std::string>("subtitle_I18n","");
            
        _myIsAnimating = false;     
        _myCurrentProject = currentProject;
         _myContentImages = _myCurrentProject->getChildrenByType(ContentImage::SPARK_TYPE);
         _myNumberOfImages = _myContentImages.size();
         if (_myNumberOfImages < 1) {
             _imageTransform0->setVisible(false);
             _imageTransform1->setVisible(false);
             _imageTransform2->setVisible(false);
             return;
         }
         _imageTransform0->setVisible(true);
         
         // bind i18n item to description widget
        _myDescription->setI18nId(myDescription_I18n);         
         _myPopUpTitle->setI18nId(myTitle_I18n);
         _myPopUpSubTitle->setI18nId(mySubTitle_I18n);
         int myHiddenPopUpHeight = POPUP_HEIGHT;//std::max(30, int(_myPopUpTitle->getTextSize()[1]));
         int myTextHeight = _myDescription->getTextSize()[1];
         _myPopupBG->getShape()->setDimensions(_myWidth, myHiddenPopUpHeight + myTextHeight);
         
         _myPopUpSubTitle->setY(myTextHeight+10);
         _myPopUpTitle->setY(myTextHeight + 10 + _myPopUpSubTitle->getTextSize()[1]);

         _myPopUpPfeil->setX(_myWidth - _myPopUpPfeil->getTextureSize()[0] - 20);
         _myPopUpPfeil->setY(myTextHeight+10);
         
         _myPopup->setY(-myTextHeight);
         _myPopupBG->setAlpha(0.5);
         //_myPopUpPfeil->setAlpha(1.0);
         
         _imageTransform0->setX(0);
         _imageTransform1->setX(_myWidth);
         _imageTransform2->setX(-_myWidth);
         _myCurrentImage = 0;
         _myDisplayedImage = 0;
         
         _image0->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[0])->getSrc());
            
         autoScaleImage(_image0);
         
         _imageTransform0->setVisible(true);
         _imageTransform1->setVisible(false);
         _imageTransform2->setVisible(false);
         
         setVisible(false);
    }
    
    void ProjectViewerImpl::autoScaleImage(ImagePtr theImage) {
        int myImageWidth = theImage->getTextureSize()[0];
        int myImageHeight = theImage->getTextureSize()[1];
        float myImageRatio = float(myImageWidth)/myImageHeight;
        
        int myWindowWidth = _myWindowPtr->getSize()[0];
        int myWindowHeight = _myWindowPtr->getSize()[1];
        float myWindowRatio = float(myWindowWidth)/myWindowHeight;
        
        int myNewWidth = 0;
        int myNewHeight = 0;
        if (myImageRatio == myWindowRatio) {
            myNewWidth = myWindowWidth;
            myNewHeight = myWindowHeight;
        } else {
            myNewWidth = myWindowWidth;
            myNewHeight = myNewWidth / myImageRatio;
            
            float myWidthScale = float(myWindowWidth) / myNewWidth;
            float myHeightScale = float(myWindowHeight) / myNewHeight;
            if (myWidthScale > myHeightScale) {
                myNewWidth *= myHeightScale;
                myNewHeight *= myHeightScale;
            } else {
                myNewWidth *= myWidthScale;
                myNewHeight *= myWidthScale;                
            }         
        }
        
        theImage->setX(float(myWindowWidth - myNewWidth) / 2.0);
        theImage->setY(float(myWindowHeight - myNewHeight) / 2.0);
        theImage->setSize(myNewWidth, myNewHeight);
    }
    
    void ProjectViewerImpl::initiateClose() {
        _imageTransform0->setVisible(false);
        _imageTransform1->setVisible(false);
        _imageTransform2->setVisible(false);
        if (_myCurrentImage == 0) {
            _imageTransform0->setVisible(true);
        } else if (_myCurrentImage == 1) {
            _imageTransform1->setVisible(true);
        } else if (_myCurrentImage == 2) {
            _imageTransform2->setVisible(true);
        }
    }
    void ProjectViewerImpl::loadInitialSet() {
         if (_myNumberOfImages > 0) {
             _image1->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[1%_myNumberOfImages])->getSrc());
             autoScaleImage(_image1);

             _image2->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[_myNumberOfImages-1])->getSrc());
             autoScaleImage(_image2);
        }        
    }
    bool ProjectViewerImpl::isPopUpOpen() {
        int myTextHeight = _myDescription->getTextSize()[1];            
        return (_myPopup->getY() != -myTextHeight);
    }
    
    void ProjectViewerImpl::onOpenClosePopup(EventPtr theEvent) {
        if (isRendered()) {
            int myTextHeight = _myDescription->getTextSize()[1];    
            animation::ParallelAnimationPtr myAnimation = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
            if (!isPopUpOpen()) {
                WidgetPropertyAnimationPtr myPosYAnimation = WidgetPropertyAnimationPtr(
                        new WidgetPropertyAnimation(_myPopup, &Widget::setY, _myPopup->getY(), 0, 300,
                            animation::EasingFnc(animation::easeInOutQuad)));
                WidgetPropertyAnimationPtr myAlphaAnimation = WidgetPropertyAnimationPtr(
                        new WidgetPropertyAnimation(_myPopupBG, &Widget::setAlpha, _myPopupBG->getAlpha(), 0.9, 300,
                            animation::EasingFnc(animation::easeInOutQuad)));
                myAnimation->add(myPosYAnimation);
                myAnimation->add(myAlphaAnimation);
                //_myPopup->setY(0);
                //_myPopup->setAlpha(0.9);
            } else {
                WidgetPropertyAnimationPtr myPosYAnimation = WidgetPropertyAnimationPtr(
                        new WidgetPropertyAnimation(_myPopup, &Widget::setY, _myPopup->getY(), -myTextHeight, 300,
                            animation::EasingFnc(animation::easeInOutQuad)));
                WidgetPropertyAnimationPtr myAlphaAnimation = WidgetPropertyAnimationPtr(
                        new WidgetPropertyAnimation(_myPopupBG, &Widget::setAlpha, _myPopupBG->getAlpha(), 0.5, 300,
                            animation::EasingFnc(animation::easeInOutQuad)));
                myAnimation->add(myPosYAnimation);
                myAnimation->add(myAlphaAnimation);
                //_myPopup->setY(-myTextHeight);
                //_myPopup->setAlpha(0.5);
            }
            animation::AnimationManager::get().play(myAnimation);            
	    }
    }
    void ProjectViewerImpl::onSwipe(EventPtr theEvent) {
        if (isRendered()) {
    	    changeImage(theEvent->getType() == "swipe-right" ? -1 :  +1);
	    }
    }
    
    void ProjectViewerImpl::changeImage(int dir) {
        if(_myIsAnimating) return;
        _myIsAnimating = true;      
        _imageTransform1->setVisible(true);
        _imageTransform2->setVisible(true);
        _myDirection =dir;
        WidgetPropertyAnimationPtr changeAnimation0 = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(_imageTransform0, &Widget::setX, _imageTransform0->getX(), _imageTransform0->getX()-_myWidth*dir, 300,
                    animation::EasingFnc(animation::easeInOutQuad)));
        WidgetPropertyAnimationPtr changeAnimation1 = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(_imageTransform1, &Widget::setX, _imageTransform1->getX(), _imageTransform1->getX()-_myWidth*dir, 300,
                    animation::EasingFnc(animation::easeInOutQuad)));
        WidgetPropertyAnimationPtr changeAnimation2 = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(_imageTransform2, &Widget::setX, _imageTransform2->getX(), _imageTransform2->getX()-_myWidth*dir, 300,
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
        if ((_myCurrentImage-_myDirection+3)%3 == 0) {
            _image0->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[(_myCurrentImage+_myDirection*2+_myNumberOfImages) %_myNumberOfImages])->getSrc());
            autoScaleImage(_image0);
        } else if ((_myCurrentImage-_myDirection+3)%3 == 1) {
            _image1->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[(_myCurrentImage+_myDirection*2+_myNumberOfImages) %_myNumberOfImages])->getSrc());
            autoScaleImage(_image1);
        } else if ((_myCurrentImage-_myDirection+3)%3 == 2) {
            _image2->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[(_myCurrentImage+_myDirection*2+_myNumberOfImages) %_myNumberOfImages])->getSrc());
            autoScaleImage(_image2);
        }
        _myCurrentImage = (_myCurrentImage + _myDirection + _myNumberOfImages) % _myNumberOfImages;
        _myIsAnimating = false;            
        
    }
    void ProjectViewerImpl::onAnimationFinished() {
        if ((_myCurrentImage-_myDirection+3)%3 == 0) {
            _imageTransform0->setX((_imageTransform0->getX() + _myWidth*_myDirection) * -1);
        } else if ((_myCurrentImage-_myDirection+3)%3 == 1) {
            _imageTransform1->setX((_imageTransform1->getX() + _myWidth*_myDirection) * -1);
        } else if ((_myCurrentImage-_myDirection+3)%3 == 2) {
            _imageTransform2->setX((_imageTransform2->getX() + _myWidth*_myDirection) * -1);
        }
    }

        
}
