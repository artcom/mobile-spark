#include "I18nHandler.h"

#include "I18nContext.h"
#include "Widget.h"

namespace spark {

    I18nHandler::I18nHandler(const masl::XMLNodePtr theXMLNode, const std::string & theKey) {
        data_ = theXMLNode->getAttributeAs<std::string>(theKey, "");
        i18nId_ = theXMLNode->getAttributeAs<std::string>("i18nId", "");
    }

    I18nHandler::~I18nHandler() {
    }

    void
    I18nHandler::realize(const WidgetPtr theWidget) {
        widget_ = theWidget;
        if (!i18nId_.empty()) {
            attachToI18nItem();
        }
    }

    void 
    I18nHandler::handleI18nOnLanguageSwitch(const EventPtr theEvent) {
        data_ = widget_->_myI18nItem->getLanguageData();
        AC_DEBUG << "on language switch data " << data_;
        widget_->_myDirtyFlag = true;
    }

    void
    I18nHandler::attachToI18nItem() {
        EventCallbackPtr myHandleLanguageSwitch = EventCallbackPtr(new I18nHandlerCB(shared_from_this(), &I18nHandler::handleI18nOnLanguageSwitch));
        if (widget_->_myI18nItem) {
            widget_->_myI18nItem->removeEventListener(I18nEvent::ON_LANGUAGE_SWITCH, myHandleLanguageSwitch);
            widget_->_myI18nItem = I18nItemPtr();
        }
        if (!i18nId_.empty()) {
            widget_->_myI18nItem = widget_->getI18nItemByName(i18nId_);
            if (!widget_->_myI18nItem) {
                throw I18nItemNotFoundException("no i18n item named " + i18nId_, PLUS_FILE_LINE);
            }
            widget_->_myI18nItem->addEventListener(I18nEvent::ON_LANGUAGE_SWITCH, myHandleLanguageSwitch);
            handleI18nOnLanguageSwitch();
        } else {
            data_ = "";
        }
        widget_->_myDirtyFlag = true;
    }
}
