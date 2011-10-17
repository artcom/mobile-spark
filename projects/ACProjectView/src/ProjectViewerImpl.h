#ifndef _included_mobile_acprojectview_ProjectViewerImpl_
#define _included_mobile_acprojectview_ProjectViewerImpl_

#include <spark/Transform.h>
#include <spark/Image.h>
#include <spark/Text.h>
#include <spark/View.h>
#include <spark/Container.h>
#include <spark/Rectangle.h>

#include <spark/SparkComponentFactory.h>

#include "ProjectImpl.h"
#include "ContentImage.h"
#include "MultiColumnText.h"


namespace acprojectview {

    class ProjectViewerImpl : public spark::Transform {
        public: 
            ProjectViewerImpl(const spark::BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~ProjectViewerImpl();
            virtual void realize();
            
            static const char * const SPARK_TYPE;             
            const static unsigned int POPUP_HEIGHT = 50;
            const static unsigned int POPUP_SIZE = 250;
            
            virtual const char * const & getType() const { return ProjectViewerImpl::SPARK_TYPE;};

            void draw();
            void showProject(ProjectImplPtr _myCurrentProject);
            void onSwipe(spark::EventPtr theEvent);
            void setWidth(int width) {_myWidth = width;};
            void setHeight(int height) {_myHeight = height;};
            void loadInitialSet();
            void initiateClose();
            bool isPopUpOpen();
            void showPopup(bool theFlag);
       private:
            unsigned int _myCurrentImage;
            unsigned int _myCurrentSlot;
            unsigned int _myNumberOfImages;
            int _myWidth;
            int _myHeight;
            int _myDisplayedImage;
            bool _myIsAnimating;
            int _myDirection;
            spark::VectorOfComponentPtr _myContentImages;
            ProjectImplPtr _myCurrentProject;
            spark::TransformPtr _imageTransform0;
            spark::TransformPtr _imageTransform1;
            spark::TransformPtr _imageTransform2;
            spark::ImagePtr _image0;
            spark::ImagePtr _image1;
            spark::ImagePtr _image2;
            spark::TransformPtr _myPopup;                
            spark::RectanglePtr _myPopupBG;                
            MultiColumnTextPtr _myDescription;
            spark::ImagePtr _myPopUpPfeil;
            spark::TextPtr _myPopUpTitle;      
            spark::TextPtr _myPopUpSubTitle;
            spark::WindowPtr _myWindowPtr;
            void changeImage(int dir);
            void onAnimationFinished();
            void onLoadNextImages();
            void onOpenClosePopup(spark::EventPtr theEvent);
            void autoScaleImage(spark::ImagePtr theImage);

            const static unsigned int TEXT_HEIGHT = 250;
            const static unsigned int POPUP_ANIMATION_DURATION = 1300;

    };
    
    typedef masl::Ptr<ProjectViewerImpl> ProjectViewerImplPtr;
    typedef masl::WeakPtr<ProjectViewerImpl> ProjectViewerImplWeakPtr;
    typedef masl::MemberFunctionCallback<ProjectViewerImpl, ProjectViewerImplWeakPtr> ProjectViewerImplCB;
    typedef spark::MemberFunctionEventCallback<ProjectViewerImpl, ProjectViewerImplWeakPtr> ProjectViewerImplEventCB;

};

#endif
