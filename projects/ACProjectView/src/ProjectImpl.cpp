#include "ProjectImpl.h"
#include "ProjectMenu.h"
#include <spark/Window.h>
#include <spark/SparkComponentFactory.h>
#include <mar/shape.h>


using namespace spark;

namespace acprojectview {
        
    const char * const ProjectImpl::SPARK_TYPE = "ProjectImpl";

    
    ProjectImpl::ProjectImpl(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
        : Transform(theApp, theXMLNode) {
    }

    ProjectImpl::~ProjectImpl() {}

    void ProjectImpl::realize() {
        Transform::realize();
        WindowPtr myWindowPtr = boost::static_pointer_cast<Window>(getRoot());
            
        std::string src = _myXMLNode->getAttributeAs<std::string>("src",""); 
        std::string title = _myXMLNode->getAttributeAs<std::string>("title",""); 
        std::string subtitle = _myXMLNode->getAttributeAs<std::string>("subtitle",""); 

        std::string myTitle_I18n = _myXMLNode->getAttributeAs<std::string>("title_I18n","");
        std::string mySubTitle_I18n =_myXMLNode->getAttributeAs<std::string>("subtitle_I18n","");
            
        imageComponent_ = boost::static_pointer_cast<Image>(getChildByName("image"));
        titleComponent_ = boost::static_pointer_cast<Text>(getChildByName("title"));
        subtitleComponent_ = boost::static_pointer_cast<Text>(getChildByName("subtitle"));
            
        unsigned myWidth = myWindowPtr->getSize()[0];
        unsigned myHeight = myWindowPtr->getSize()[1];
        ProjectMenuPtr myMenu =  boost::static_pointer_cast<ProjectMenu>(getParent());
        int myHorizontalTiling = myMenu->getHorizontalTiling();
        int myVerticalTiling = myMenu->getVerticalTiling();

        int dx = myWidth / myHorizontalTiling;
        int dy = myHeight / myVerticalTiling;
        int iconWidth = dx - myMenu->getGapX();
        int iconHeight = dy - myMenu->getGapY();
        
        if (src.size() >0 ) {
            imageComponent_->setSrc(src);
        }
         
        if (myTitle_I18n.size() > 0) {
           titleComponent_->setMaxWidth(iconWidth-titleComponent_->getX()*2);
           titleComponent_->setI18nId(myTitle_I18n);           
        }
        if (mySubTitle_I18n.size() > 0) {
            subtitleComponent_->setMaxWidth(iconWidth-subtitleComponent_->getX()*2);
            subtitleComponent_->setI18nId(mySubTitle_I18n);
        }
        
        titleComponent_->setY(subtitleComponent_->getTextSize()[1]);
        
        //imageComponent_->setSize(iconWidth, iconHeight);

        
        
        //int textSpace = titleComponent_->getTextSize()[1] + subtitleComponent_->getTextSize()[1];
        // set Position:
        //setX(myMenu->getGapX()/2 + dx * (i/myVerticalTiling)); 
        //setY(myMenu->getGapY()/2 + dy * (i % myVerticalTiling));
        // scale preview image:
        
//        float scaleX = iconWidth / (imageComponent_->getTextureSize()[0]);
//        //float scaleY = (iconHeight - textSpace) / (imageComponent_->getTextureSize()[1]);
//        float scaleY = iconHeight / (imageComponent_->getTextureSize()[1]);
//        float scale = std::max(scaleX, scaleY);
//        imageComponent_->setScaleX(scale);
//        imageComponent_->setScaleY(scale);
        imageComponent_->fitToSize(iconWidth, iconHeight);
         // center image: 
        //imageComponent_->setX( 0);//(iconWidth - scale * imageComponent_->getTextureSize()[0])/2.0);
        imageComponent_->setY( 0);//(textSpace + iconHeight - scale * imageComponent_->getTextureSize()[1])/2.0);
        
    }    
}
