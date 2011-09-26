#include "spark/Image.h"
#include "ProjectMenu.h"

#include "ProjectImpl.h"



using namespace spark;

namespace acprojectview {

    ProjectMenu::ProjectMenu(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
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
        ProjectMenuPtr myPtr = boost::static_pointer_cast<ProjectMenu>(shared_from_this());
        spark::EventCallbackPtr dragCB = EventCallbackPtr(new ProjectMenuEventCB(myPtr, &ProjectMenu::onDragCB));
        _myWindowPtr->addEventListener(GestureEvent::PAN, dragCB);
        spark::EventCallbackPtr fingerDownCB = EventCallbackPtr(new ProjectMenuEventCB(myPtr, &ProjectMenu::onFingerDownCB));
        _myWindowPtr->addEventListener(TouchEvent::DOWN, fingerDownCB);
        
    }

    ProjectMenu::~ProjectMenu() {}

    void
    ProjectMenu::arrangeProjects() {

        ProjectMenuPtr myPtr = boost::static_pointer_cast<ProjectMenu>(shared_from_this());

        int theWidth = _myWindowPtr->getSize()[0];
        int theHeight = _myWindowPtr->getSize()[1];
        int dx = theWidth / _myHorizontalTiling;
        int dy = theHeight / _myVerticalTiling;
        int iconWidth = dx - _myGapX;
        int iconHeight = dy - _myGapY;
        //ProjectMenuPtr _myPtr = boost::static_pointer_cast<ProjectMenu>(shared_from_this());
        
        const VectorOfComponentPtr & myChildren = myPtr->getChildrenByType(ProjectImpl::SPARK_TYPE);
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
    
    void
    ProjectMenu::onDragCB(EventPtr theEvent) {
        ProjectMenuPtr myPtr = boost::static_pointer_cast<ProjectMenu>(shared_from_this());
        if(myPtr->getX() > 0) {
            myPtr->setX(0);
            return;
        }
        GestureEventPtr myEvent = boost::static_pointer_cast<GestureEvent>(theEvent);
        myPtr->setX(_myStartDragX + myEvent->getTranslateX());
        

    }
    
    void
    ProjectMenu::onFingerDownCB(EventPtr theEvent) {
        ProjectMenuPtr myPtr = boost::static_pointer_cast<ProjectMenu>(shared_from_this());
        _myStartDragX = myPtr->getX();
    }
    
}
