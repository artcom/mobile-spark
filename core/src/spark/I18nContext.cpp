#include "I18nContext.h"

namespace spark {


    ///////////////////////////////////////////////////////I18nContext
    const char * const I18nContext::SPARK_TYPE = "I18nContext";

    I18nContext::I18nContext(const spark::BaseAppPtr& theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        Container(theApp, theXMLNode, theParent), language_(NO_LANGUAGE)
    {
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
    }

    void
    I18nContext::addChild(const ComponentPtr theChild) {
        Container::addChild(theChild);
        I18nItemPtr myChild = boost::static_pointer_cast<I18nItem>(theChild);
        if (!myChild) {
            throw I18nItemNotFoundException("adding child " + theChild->getName() + " to I18nContext " + getName() + " does not work because theChild is not an I18nItem", PLUS_FILE_LINE);
        }
        if (language_ != NO_LANGUAGE) {
            myChild->switchLanguage(language_);
        }
    }




    ///////////////////////////////////////////////////////I18nItem
    const char * const I18nItem::SPARK_TYPE = "I18nItem";
        
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
        EventPtr myEvent = EventPtr(new I18nEvent(I18nEvent::ON_LANGUAGE_SWITCH, shared_from_this()));
        dispatchEvent(myEvent);
    }

    std::string 
    I18nItem::getLanguageData(const LANGUAGE theLanguage) const {
        std::string myData = "";
        const LANGUAGE myLanguage = (theLanguage != NO_LANGUAGE ? theLanguage : language_);
        std::map<LANGUAGE, std::string>::const_iterator it = languageData_.find(myLanguage);
        if (it != languageData_.end()) {
            myData = it->second;
        }
        return myData;
    }
}


