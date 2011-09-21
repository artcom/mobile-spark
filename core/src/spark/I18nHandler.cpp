#include "I18nHandler.h"

#include "I18nContext.h"
#include "Container.h"

namespace spark {

    I18nHandler::I18nHandler(const masl::XMLNodePtr theXMLNode, const std::string & theKey) {
        data_ = theXMLNode->getAttributeAs<std::string>(theKey, "");
        i18nId_ = theXMLNode->getAttributeAs<std::string>("i18nId", "");
    }

    I18nHandler::~I18nHandler() {
    }

    void
    I18nHandler::realize(const ContainerPtr theContainer) {
        container_ = theContainer;
        if (i18nId_.size() > 0) {
            attachToI18nItem();
        }
    }

    void 
    I18nHandler::handleI18nOnLanguageSwitch(const EventPtr theEvent) {
        data_ = container_->_myI18nItem->getLanguageData();
        AC_DEBUG << "on language switch data " << data_;
        container_->_myDirtyFlag = true;
    }

    void
    I18nHandler::attachToI18nItem() {
        EventCallbackPtr myHandleLanguageSwitch = EventCallbackPtr(new I18nHandlerCB(shared_from_this(), &I18nHandler::handleI18nOnLanguageSwitch));
        if (container_->_myI18nItem) {
            container_->_myI18nItem->removeEventListener(I18nEvent::ON_LANGUAGE_SWITCH, myHandleLanguageSwitch);
            container_->_myI18nItem = I18nItemPtr();
        }
        if (i18nId_.size() > 0) {
            container_->_myI18nItem = container_->getI18nItemByName(i18nId_);
            if (!container_->_myI18nItem) {
                throw I18nItemNotFoundException("no i18n item named " + i18nId_, PLUS_FILE_LINE);
            }
            container_->_myI18nItem->addEventListener(I18nEvent::ON_LANGUAGE_SWITCH, myHandleLanguageSwitch);
            handleI18nOnLanguageSwitch();
        } else {
            data_ = "";
        }
        container_->_myDirtyFlag = true;
    }
}
