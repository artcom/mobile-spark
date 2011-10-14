#include "ProjectViewerImpl.h"

#include <spark/Window.h>
#include <spark/Rectangle.h>
#include <spark/SparkComponentFactory.h>
#include <animation/AnimationManager.h>
#include <animation/ParallelAnimation.h>
#include <animation/SequenceAnimation.h>
#include <animation/DelayAnimation.h>
#include <animation/Easing.h>

#include <masl/numeric_functions.h>
#include <boost/progress.hpp>

using namespace spark;
using namespace std;
using namespace masl;

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
        _myDescription = boost::static_pointer_cast<MultiColumnText>(_myPopup->getChildByName("description"));
        _myPopUpTitle = boost::static_pointer_cast<Text>(_myPopup->getChildByName("title"));
        _myPopUpSubTitle = boost::static_pointer_cast<Text>(_myPopup->getChildByName("subtitle"));
        _myPopUpPfeil = boost::static_pointer_cast<Image>(_myPopup->getChildByName("pfeil"));

        _myPopUpTitle->setText("");   
        _myPopUpSubTitle->setText("");   
        
    }

    ProjectViewerImpl::~ProjectViewerImpl() {}

    void
    ProjectViewerImpl::realize() {
        Transform::realize();
        ProjectViewerImplPtr ptr = boost::static_pointer_cast<ProjectViewerImpl>(shared_from_this());
        spark::EventCallbackPtr mySwipeCB = EventCallbackPtr(new ProjectViewerImplEventCB(ptr, &ProjectViewerImpl::onSwipe));
        getRoot()->addEventListener(GestureEvent::SWIPE_LEFT, mySwipeCB);
        getRoot()->addEventListener(GestureEvent::SWIPE_RIGHT, mySwipeCB);

    
        spark::EventCallbackPtr mySwipeUpDownCB = EventCallbackPtr(new ProjectViewerImplEventCB(ptr, &ProjectViewerImpl::onOpenClosePopup));
        _myPopupBG->addEventListener(TouchEvent::PICKED, mySwipeUpDownCB, true);
        _myPopUpPfeil->addEventListener(TouchEvent::PICKED, mySwipeUpDownCB, true);
            
        _myWindowPtr = boost::static_pointer_cast<Window>(getRoot());                
        _myWidth = _myWindowPtr->getSize()[0];
        _myHeight = _myWindowPtr->getSize()[1];

        boost::static_pointer_cast<Rectangle>(getChildByName("background"))->setSize(vector2(_myWidth,_myHeight));
        
        _myPopUpTitle->setMaxWidth(_myWidth - (2*_myPopUpTitle->getX()));
        _myPopUpSubTitle->setMaxWidth(_myWidth - (2*_myPopUpTitle->getX()));
        
        _myPopupBG->setSize(_myWidth, 0);
            
    }

    void ProjectViewerImpl::showProject(ProjectImplPtr currentProject) {  
        //boost::timer::timer myTimer;
                      
        showPopup(false);            
        _myCurrentProject = currentProject;
            
        _myIsAnimating = false;     
         _myContentImages = _myCurrentProject->getChildrenByType(ContentImage::SPARK_TYPE);
         _myNumberOfImages = _myContentImages.size();
         if (_myNumberOfImages < 1) {
             _imageTransform0->setVisible(false);
             _imageTransform1->setVisible(false);
             _imageTransform2->setVisible(false);
             return;
         }
         _imageTransform0->setVisible(true);
                  
         _imageTransform0->setX(0);
         _imageTransform1->setX(_myWidth);
         _imageTransform2->setX(-_myWidth);
         _myCurrentImage = 0;
         _myDisplayedImage = 0;
         _myCurrentSlot=0;
        
         //_image0->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[0])->getSrc());            
         ImagePtr myProjectEntry =  boost::static_pointer_cast<Image>(currentProject->getChildByName("image"));
         _image0->setSrc(myProjectEntry->getSrc());            
         autoScaleImage(_image0);

         _imageTransform1->setVisible(false);
         _imageTransform2->setVisible(false);
         
         setVisible(false);
         //AC_PRINT << "******************ProjectViewerImpl::showProject***************************** " << myTimer.elapsed();
         
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
    void ProjectViewerImpl::showPopup(bool theFlag) {       
        _myPopup->setVisible(theFlag);        
    }
    void ProjectViewerImpl::loadInitialSet() {        
        string myTitle_I18n = _myCurrentProject->getNode()->getAttributeAs<std::string>("title_I18n","");
        string mySubTitle_I18n = _myCurrentProject->getNode()->getAttributeAs<std::string>("subtitle_I18n","");
         _myPopUpTitle->setI18nId(myTitle_I18n);
         _myPopUpSubTitle->setI18nId(mySubTitle_I18n);         

         int myHiddenPopUpHeight = POPUP_HEIGHT;//std::max(30, int(_myPopUpTitle->getTextSize()[1]));
         int myTextHeight = 250;
         _myPopupBG->setSize(_myWidth, myHiddenPopUpHeight + myTextHeight);
         
         _myPopUpSubTitle->setY(myTextHeight+10);
         _myPopUpTitle->setY(myTextHeight + 10 + _myPopUpSubTitle->getTextSize()[1]);

         _myPopUpPfeil->setX(_myWidth - (_myPopUpPfeil->getTextureSize()[0]/2.0) - 20);
         _myPopUpPfeil->setY((50 + (_myPopUpPfeil->getTextureSize()[1]/2.0) ) /2.0  + myTextHeight - _myPopUpPfeil->getTextureSize()[1]);
        _myPopup->setY(-myTextHeight);

        _myDescription->setMaxWidth( (_myWidth/2.0) - _myDescription->getColumnSpace() - _myDescription->getColumnSpace()/2.0);

        string myDescription_I18n = _myCurrentProject->getNode()->getAttributeAs<std::string>("description_I18n","");        
        _myDescription->setI18nId(myDescription_I18n);         
        
        
         if (_myNumberOfImages > 0) {
             _image1->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[1%_myNumberOfImages])->getSrc());
             autoScaleImage(_image1);

             _image2->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[_myNumberOfImages-1])->getSrc());
             autoScaleImage(_image2);
        }        
    }
    bool ProjectViewerImpl::isPopUpOpen() {
        int myTextHeight = 250;
        return (_myPopup->getY() != -myTextHeight);
    }
    
    void ProjectViewerImpl::onOpenClosePopup(EventPtr theEvent) {
        AC_PRINT << "onOpenClosePopup";
        if (isRendered()) {
            int myTextHeight = 250;
            animation::ParallelAnimationPtr myAnimation = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
            if (!isPopUpOpen()) {
                WidgetPropertyAnimationPtr myPosYAnimation = WidgetPropertyAnimationPtr(
                        new WidgetPropertyAnimation(_myPopup, &Widget::setY, _myPopup->getY()-1, 0, 300,
                            animation::EasingFnc(animation::easeInOutQuad)));
                WidgetPropertyAnimationPtr myAlphaAnimation = WidgetPropertyAnimationPtr(
                        new WidgetPropertyAnimation(_myPopupBG, &Widget::setAlpha, _myPopupBG->getAlpha(), 0.9, 300,
                            animation::EasingFnc(animation::easeInOutQuad)));
                WidgetPropertyAnimationPtr myRotateAnimation = WidgetPropertyAnimationPtr(
                        new WidgetPropertyAnimation(_myPopUpPfeil, &Widget::setRotationZ, _myPopUpPfeil->getRotationZ(), M_PI, 300,
                            animation::EasingFnc(animation::easeInOutQuad)));
                                
                myAnimation->add(myRotateAnimation);
                myAnimation->add(myPosYAnimation);
                myAnimation->add(myAlphaAnimation);
            } else {
                _myDescription->reset();
                WidgetPropertyAnimationPtr myPosYAnimation = WidgetPropertyAnimationPtr(
                        new WidgetPropertyAnimation(_myPopup, &Widget::setY, _myPopup->getY(), -myTextHeight, 300,
                            animation::EasingFnc(animation::easeInOutQuad)));
                WidgetPropertyAnimationPtr myAlphaAnimation = WidgetPropertyAnimationPtr(
                        new WidgetPropertyAnimation(_myPopupBG, &Widget::setAlpha, _myPopupBG->getAlpha(), 0.5, 300,
                            animation::EasingFnc(animation::easeInOutQuad)));
                WidgetPropertyAnimationPtr myRotateAnimation = WidgetPropertyAnimationPtr(
                        new WidgetPropertyAnimation(_myPopUpPfeil, &Widget::setRotationZ, _myPopUpPfeil->getRotationZ(), 0, 300,
                            animation::EasingFnc(animation::easeInOutQuad)));
                myAnimation->add(myRotateAnimation);
                myAnimation->add(myPosYAnimation);
                myAnimation->add(myAlphaAnimation);
            }
            animation::AnimationManager::get().play(myAnimation);            
	    }
    }
    void ProjectViewerImpl::onSwipe(EventPtr theEvent) {
        if (isRendered()) {
            if (isPopUpOpen()) {
    	        _myDescription->swipe((theEvent->getType() == "swipe-right" ? -1 :  +1));
	        } else {
    	        changeImage(theEvent->getType() == "swipe-right" ? -1 :  +1);
	        }
        }
    }
    
    void ProjectViewerImpl::changeImage(int dir) {
        if(_myIsAnimating) return;
        _myIsAnimating = true;      
        _imageTransform1->setVisible(true);
        _imageTransform2->setVisible(true);
        _myDirection =dir;
        _myCurrentSlot = (_myCurrentSlot + dir+3) %3;
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
        myAnimation->setOnFinish(masl::CallbackPtr(new ProjectViewerImplCB(ptr, &ProjectViewerImpl::onAnimationFinished)));
        myAnimation->add(changeAnimation0);
        myAnimation->add(changeAnimation1);
        myAnimation->add(changeAnimation2);

        animation::DelayAnimationPtr myReadNextImagesAnim = animation::DelayAnimationPtr(new animation::DelayAnimation(0));
        myReadNextImagesAnim->setOnFinish(masl::CallbackPtr(new ProjectViewerImplCB(ptr, &ProjectViewerImpl::onLoadNextImages)));
            
        mySeqAnimation->add(myAnimation);
        mySeqAnimation->add(myReadNextImagesAnim);
        animation::AnimationManager::get().play(mySeqAnimation);
    }
    
    void ProjectViewerImpl::onLoadNextImages() {
        if ((_myCurrentSlot + _myDirection+3)%3 == 0) {
            _image0->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[(_myCurrentImage+_myDirection*2+_myNumberOfImages) %_myNumberOfImages])->getSrc());
            autoScaleImage(_image0);
        } else if ((_myCurrentSlot + _myDirection+3)%3 == 1) {
            _image1->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[(_myCurrentImage+_myDirection*2+_myNumberOfImages) %_myNumberOfImages])->getSrc());
            autoScaleImage(_image1);
        } else if ((_myCurrentSlot + _myDirection+3)%3 == 2) {
            _image2->setSrc(boost::static_pointer_cast<ContentImage>(_myContentImages[(_myCurrentImage+_myDirection*2+_myNumberOfImages) %_myNumberOfImages])->getSrc());
            autoScaleImage(_image2);
        }
        _myCurrentImage = (_myCurrentImage + _myDirection + _myNumberOfImages) % _myNumberOfImages;
        _myIsAnimating = false;            
        
    }
    void ProjectViewerImpl::onAnimationFinished() {
        if (_imageTransform0->getX() > _myWidth+1) _imageTransform0->setX(-_myWidth);
        if (_imageTransform1->getX() > _myWidth+1) _imageTransform1->setX(-_myWidth);
        if (_imageTransform2->getX() > _myWidth+1) _imageTransform2->setX(-_myWidth);
        if (_imageTransform0->getX() < -_myWidth-1) _imageTransform0->setX(_myWidth);
        if (_imageTransform1->getX() < -_myWidth-1) _imageTransform1->setX(_myWidth);
        if (_imageTransform2->getX() < -_myWidth-1) _imageTransform2->setX(_myWidth);
    }

        
}
