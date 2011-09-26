#include <spark/SparkComponentFactory.h>
#include "spark/Image.h"
#include "ProjectMenu.h"

#include "ProjectImpl.h"



using namespace spark;

namespace acprojectview {

    ProjectMenu::ProjectMenu(const BaseAppPtr theApp, const XMLNodePtr theXMLNode)
        : Transform(theApp, theXMLNode) {
        _myHorizontalTiling = theXMLNode->getAttributeAs<int>("x-tiling",0); 
        _myVerticalTiling = theXMLNode->getAttributeAs<int>("y-tiling",0); 
        _myGapX = theXMLNode->getAttributeAs<int>("x-gap",0); 
        _myGapY = theXMLNode->getAttributeAs<int>("y-gap",0); 
        
    }
    
    void
    ProjectMenu::realize() {
        Transform::realize();
        _myWindowPtr = boost::static_pointer_cast<Window>(getRoot());
    }

    ProjectMenu::~ProjectMenu() {}

    void
    ProjectMenu::arrangeProjects() {
        int theWidth = _myWindowPtr->getSize()[0];
        int theHeight = _myWindowPtr->getSize()[1];
        int dx = theWidth / _myHorizontalTiling;
        int dy = theHeight / _myVerticalTiling;
        int iconWidth = dx - _myGapX;
        int iconHeight = dy - _myGapY;
         
        ProjectMenuPtr ptr = boost::static_pointer_cast<ProjectMenu>(shared_from_this());

        const VectorOfComponentPtr & myChildren = ptr->getChildrenByType(ProjectImpl::SPARK_TYPE);
        for (size_t i = 0; i < myChildren.size(); i++) {
            ProjectImplPtr myProject = boost::static_pointer_cast<ProjectImpl>(myChildren[i]);
            // set Position:
            myProject->setX(_myGapX/2 + dx * (i/_myVerticalTiling)); 
            myProject->setY(_myGapY/2 + dy * (i % _myVerticalTiling));
            // scale preview image:
            ImagePtr image = boost::static_pointer_cast<spark::Image>(myProject->getChildByName("image"));
            int textHeight = boost::static_pointer_cast<spark::Image>(myProject->getChildByName("title"))->getX();
            float scaleX = iconWidth / (image->getTextureSize()[0]);
            float scaleY = iconHeight / (image->getTextureSize()[1] + textHeight*2);
            float scale = std::min(scaleX, scaleY);
            image->setScaleX(scale);
            image->setScaleY(scale);
        }
        
    }
    
    int ProjectMenu::getPreviewWidth() {
        return 200;
    }
    
    int ProjectMenu::getPreviewHeight(){
        return 200;
    }
    
}
