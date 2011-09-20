#include "I18nContext.h"

namespace spark {


    ///////////////////////////////////////////////////////I18nContext
    const char * const I18nContext::SPARK_TYPE = "I18nContext";

    I18nContext::I18nContext(const spark::BaseAppPtr& theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        Container(theApp, theXMLNode, theParent), language_(NO_LANGUAGE) {
        for (std::vector<ComponentPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
            addChild(*it, false);
        }
        std::string myLanguage = _myXMLNode->getAttributeAs<std::string>("defaultLanguage", I18nConstants::LANGUAGE_STRINGS[EN]); 
        defaultLanguage_ = I18nConstants::getLanguageId(myLanguage);
        needsUpdate_ = true;
    }

    I18nContext::~I18nContext() {
    }

    void
    I18nContext::setup() {
        if (needsUpdate_) {
            needsUpdate_ = false;
            switchLanguage(defaultLanguage_);
        }
    }

    void
    I18nContext::switchLanguage(const LANGUAGE theLanguage) {
        AC_DEBUG << "I18n context " << getName() << " switching to language " << theLanguage << " _myLanguage: " << language_;
        if (language_ != theLanguage) {
            language_ = theLanguage;
            for (std::vector<ComponentPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
                I18nItemPtr myItem = boost::static_pointer_cast<I18nItem>(*it);
                if (myItem) myItem->switchLanguage(theLanguage);
            }
        }
        EventPtr myEvent = EventFactory::get().handleEvent("<" + std::string(I18nEvent::CLASSNAME) + " type=\"" + std::string(I18nEvent::ON_LANGUAGE_SWITCH) + "\"/>");
        myEvent->connect(getApp()->_mySparkWindow);
        (*myEvent)();
    }

    void
    I18nContext::addChild(const ComponentPtr theChild, const bool theSetParentFlag) {
        I18nItemPtr myChild = boost::static_pointer_cast<I18nItem>(theChild);
        if (!myChild) {
            throw I18nItemNotFoundException("adding child " + theChild->getName() + " to I18nContext " + getName() + " does not work because theChild is not an I18nItem", PLUS_FILE_LINE);
        }
        if (theSetParentFlag) {
            Container::addChild(theChild);
        }
        if (language_ != NO_LANGUAGE) {
            myChild->switchLanguage(language_);
        }
    }




    ///////////////////////////////////////////////////////I18nItem
    I18nItem::I18nItem(const spark::BaseAppPtr & theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
              Component(theXMLNode, theParent){
        for (std::vector<XMLNodePtr>::iterator it = theXMLNode->children.begin(); it != theXMLNode->children.end(); ++it) {
            LANGUAGE myLanguage = I18nConstants::getLanguageId((*it)->nodeName);
            languageData_[myLanguage] = (*it)->content;
        }
    }

    I18nItem::~I18nItem() {
    }

    void 
    I18nItem::switchLanguage(const LANGUAGE theLanguage) {
        language_ = theLanguage;
    }

    std::string 
    I18nItem::getLanguageData(const LANGUAGE theLanguage) const {
        std::string myData = "";
        const LANGUAGE myLanguage = (theLanguage != NO_LANGUAGE ? theLanguage : language_);
        if (languageData_.find(myLanguage) != languageData_.end()) {
            myData = languageData_.at(myLanguage);
        }
        return myData;
    }


    ///////////////////////////////////////////////////////I18nText
    const char * const I18nText::SPARK_TYPE = "I18nText";
    I18nText::I18nText(const spark::BaseAppPtr & theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
              I18nItem(theApp, theXMLNode, theParent) {
    }

    I18nText::~I18nText() {
    }

    std::string   
    I18nText::getText() {
        return getLanguageData();
    }
}


