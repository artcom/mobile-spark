#include "I18nHandler.h"

#include <masl/XMLNode.h>

#include "I18nContext.h"
#include "Widget.h"

namespace spark {

    I18nHandler::I18nHandler(): data_(""),  i18nId_(""){
            
    }
    
    I18nHandler::~I18nHandler() {
        if (widget_&& widget_->_myI18nItem && _myHandleI18nEventCB && widget_->_myI18nItem->hasEventListener(I18nEvent::ON_LANGUAGE_SWITCH, _myHandleI18nEventCB) ) {
            widget_->_myI18nItem->removeEventListener(I18nEvent::ON_LANGUAGE_SWITCH, _myHandleI18nEventCB);
        }
    }
    
    void 
    I18nHandler::setI18nData(std::string theData) {
        data_ = theData;
    }
    
    void 
    I18nHandler::setI18nId(std::string theNewI18nId) {
        i18nId_ = theNewI18nId;
        if (!i18nId_.empty()) {
            attachToI18nItem();
        }
    }

    void
    I18nHandler::realize(const WidgetPtr theWidget, EventCallbackPtr theHandleLanguageSwitch) {
        _myHandleI18nEventCB = theHandleLanguageSwitch;
        widget_ = theWidget;
        if (!i18nId_.empty()) {
            attachToI18nItem();
        }
    }
    
    void
    I18nHandler::attachToI18nItem() {
        if (widget_) {
            if (widget_->_myI18nItem) {
                if (widget_->_myI18nItem->hasEventListener(I18nEvent::ON_LANGUAGE_SWITCH, _myHandleI18nEventCB)) {
                    widget_->_myI18nItem->removeEventListener(I18nEvent::ON_LANGUAGE_SWITCH, _myHandleI18nEventCB);
                }
                widget_->_myI18nItem = I18nItemPtr();
            }
            if (!i18nId_.empty()) {
                widget_->_myI18nItem = widget_->getI18nItemByName(i18nId_);
                if (!widget_->_myI18nItem) {
                    throw I18nItemNotFoundException("no i18n item named " + i18nId_, PLUS_FILE_LINE);
                }
                widget_->_myI18nItem->addEventListener(I18nEvent::ON_LANGUAGE_SWITCH, _myHandleI18nEventCB);
                _myHandleI18nEventCB->execute(spark::EventPtr());
            } else {
                data_ = "";
            }
            widget_->_myDirtyFlag = true;
        }
    }
}
