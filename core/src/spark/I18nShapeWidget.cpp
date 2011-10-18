#include "I18nContext.h"
#include "I18nShapeWidget.h"

namespace spark {
    I18nShapeWidget::I18nShapeWidget(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
        : ShapeWidget(theApp, theXMLNode), 
        i18nId_(theXMLNode->getAttributeAs<std::string>("i18nId", ""))    {
    }

    I18nShapeWidget::~I18nShapeWidget() {
        AC_INFO << "delete I18nShapeWidget " << getName();
        if (_myI18nItem && _myI18nItem->hasEventListener(I18nEvent::ON_LANGUAGE_SWITCH, _myHandleLanguageSwitch) ) {
            _myI18nItem->removeEventListener(I18nEvent::ON_LANGUAGE_SWITCH, _myHandleLanguageSwitch);
        }
    }
    
    void 
    I18nShapeWidget::setI18nData(const std::string & theData) {
        data_ = theData;
    }
    
    void 
    I18nShapeWidget::setI18nId(const std::string & theNewI18nId) {
        i18nId_ = theNewI18nId;
        attachToI18nItem();
    }

    void
    I18nShapeWidget::realize()  {
        ShapeWidget::realize();
        I18nShapeWidgetPtr ptr = boost::static_pointer_cast<I18nShapeWidget>(shared_from_this());        
        _myHandleLanguageSwitch = EventCallbackPtr(new I18nWidgetEventCallback(ptr, &I18nShapeWidget::handleI18nOnLanguageSwitch));
        if (!i18nId_.empty()) {
            attachToI18nItem();
        }            
    }

    void 
    I18nShapeWidget::handleI18nOnLanguageSwitch(const EventPtr theEvent) {
        setI18nData(getI18nItem()->getLanguageData());
        _myDirtyFlag = true;
        AC_DEBUG << "on language switch data " << data_;
    }

    void
    I18nShapeWidget::attachToI18nItem() {
        AC_PRINT << "attach1";
        if (_myI18nItem) {
            if (_myI18nItem->hasEventListener(I18nEvent::ON_LANGUAGE_SWITCH, _myHandleLanguageSwitch)) {
                _myI18nItem->removeEventListener(I18nEvent::ON_LANGUAGE_SWITCH, _myHandleLanguageSwitch);
            }
            _myI18nItem = I18nItemPtr();
        }
        AC_PRINT << "attach2";
        if (!i18nId_.empty()) {
        AC_PRINT << "attach3";
            _myI18nItem = getI18nItemByName(i18nId_);
        AC_PRINT << "attach4";
            if (!_myI18nItem) {
                throw I18nItemNotFoundException("no i18n item named " + i18nId_, PLUS_FILE_LINE);
            }
        AC_PRINT << "attach5";
            _myI18nItem->addEventListener(I18nEvent::ON_LANGUAGE_SWITCH, _myHandleLanguageSwitch);
        AC_PRINT << "attach6";
            _myHandleLanguageSwitch->execute(spark::EventPtr());
        AC_PRINT << "attach7";
        } else {
            data_ = "";
        }
        _myDirtyFlag = true;
    }
}
