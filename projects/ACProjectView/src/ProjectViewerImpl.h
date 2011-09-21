#ifndef _included_mobile_acprojectview_ProjectViewerImpl_
#define _included_mobile_acprojectview_ProjectViewerImpl_

#include <spark/Transform.h>
#include <spark/Image.h>
#include <spark/Text.h>
#include <spark/View.h>
#include <spark/Container.h>
#include <spark/SparkComponentFactory.h>

#include "ProjectImpl.h"
#include "ContentImage.h"
#include "Description.h"






namespace acprojectview {

    class ProjectViewerImpl : public spark::Transform {
        public: 
            ProjectViewerImpl(const spark::BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode, spark::ComponentPtr theParent);
            virtual ~ProjectViewerImpl();
            virtual void realize();
            
            static const char * const SPARK_TYPE;             
            virtual const char * const & getType() const { return ProjectViewerImpl::SPARK_TYPE;};
             
            void draw();
            void showProject(ProjectImplPtr _myCurrentProject);
            void onSwipe(spark::EventPtr theEvent);
            void setWidth(int width) {_myWidth = width;};
            void setHeight(int height) {_myHeight = height;};


       private:
            unsigned int _myCurrentImage;
            unsigned int _myNumberOfImages;
            int _myWidth;
            int _myHeight;
            int _myDisplayedImage;
            bool _myIsAnimating;
            int _myDirection;
            spark::VectorOfComponentPtr _myContentImages;
            //std::vector<ContentImage> _myContentImages;
            ProjectImplPtr _myCurrentProject;
            spark::ImagePtr image0;
            spark::ImagePtr image1;
            spark::ImagePtr image2;
            spark::TextPtr _myDescription;

            
            void changeImage(int dir);
            void onAnimationFinished();


    };
    
    typedef boost::shared_ptr<ProjectViewerImpl> ProjectViewerImplPtr;
    typedef spark::MemberFunctionEventCallback<ProjectViewerImpl, ProjectViewerImplPtr> ProjectViewerImplCB;

};

#endif
