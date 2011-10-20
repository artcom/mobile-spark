#include "ProjectImpl.h"
#include "ProjectMenu.h"
#include "MultiColumnText.h"

#include <spark/Window.h>
#include <spark/Rectangle.h>
#include <spark/SparkComponentFactory.h>


using namespace spark;

namespace acprojectview {
        
    const char * const ProjectImpl::SPARK_TYPE = "ProjectImpl";

    
    ProjectImpl::ProjectImpl(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
        : Transform(theApp, theXMLNode) {
    }

    ProjectImpl::~ProjectImpl() {}

    void ProjectImpl::realize() {
        Transform::realize();
        std::string src = _myXMLNode->getAttributeAs<std::string>("src",""); 
        std::string title = _myXMLNode->getAttributeAs<std::string>("title",""); 
        std::string subtitle = _myXMLNode->getAttributeAs<std::string>("subtitle",""); 

        std::string myTitle_I18n = _myXMLNode->getAttributeAs<std::string>("title_I18n","");
        std::string mySubTitle_I18n =_myXMLNode->getAttributeAs<std::string>("subtitle_I18n","");
            
        imageComponent_ = boost::static_pointer_cast<Image>(getChildByName("image"));
        titleComponent_ = boost::static_pointer_cast<Text>(getChildByName("title"));
        subtitleComponent_ = boost::static_pointer_cast<Text>(getChildByName("subtitle"));
                
        if (src.size() >0 ) {
            imageComponent_->setSrc(src);
        }
         
        if (myTitle_I18n.size() > 0) {
            titleComponent_->setI18nId(myTitle_I18n);           
        }
        if (mySubTitle_I18n.size() > 0) {
            subtitleComponent_->setI18nId(mySubTitle_I18n);
            subtitleComponent_->getTextSize();
        }
        fitToSize();

        /*std::string myI18n(getNode()->getAttributeAs<std::string>("description_I18n",""));
        AC_PRINT << "preload project: " << getName() << " i18n: " << myI18n;
        MultiColumnTextPtr myDescription = boost::static_pointer_cast<MultiColumnText>(getChildByName("dummy_description"));
        myDescription->setI18nId(myI18n);*/
    }
        
    void ProjectImpl::onResume() {
        Transform::onResume();
        fitToSize();
    }
    
    void ProjectImpl::fitToSize() {
        WindowPtr myWindowPtr = boost::static_pointer_cast<Window>(getRoot());
        unsigned myWidth = myWindowPtr->getSize()[0];
        unsigned myHeight = myWindowPtr->getSize()[1];
        ProjectMenuPtr myMenu =  boost::static_pointer_cast<ProjectMenu>(getParent().lock());
        int myHorizontalTiling = myMenu->getHorizontalTiling();
        int myVerticalTiling = myMenu->getVerticalTiling();

        int iconWidth = (myWidth-(myHorizontalTiling-1)*(myMenu->getGapX()-1)) / myHorizontalTiling;
        int iconHeight = (myHeight-(myVerticalTiling-1)*(myMenu->getGapY()-1)) / myVerticalTiling;
         
        titleComponent_->setMaxWidth(iconWidth-titleComponent_->getX()*2);
        subtitleComponent_->setMaxWidth(iconWidth-subtitleComponent_->getX()*2);
        
        boost::static_pointer_cast<Rectangle>(getChildByName("textplane"))->setSize(vector2(iconWidth, 50));
        imageComponent_->fitToSize(iconWidth, iconHeight);
        imageComponent_->setY( 0);//(textSpace + iconHeight - scale * imageComponent_->getTextureSize()[1])/2.0);
        
    }    
}
