#include "I18nContext.h"
#include "I18nShapeWidget.h"

namespace spark {
    I18nShapeWidget::I18nShapeWidget(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
        : ShapeWidget(theApp, theXMLNode), 
        i18nId_(theXMLNode->getAttributeAs<std::string>("i18nId", ""))    {
    }

    I18nShapeWidget::~I18nShapeWidget() {
        I18nShapeWidgetPtr ptr = boost::static_pointer_cast<I18nShapeWidget>(shared_from_this());        
        EventCallbackPtr myHandleLanguageSwitch = EventCallbackPtr(new I18nWidgetEventCallback(ptr, &I18nShapeWidget::handleI18nOnLanguageSwitch));
        
        if (_myI18nItem && _myI18nItem->hasEventListener(I18nEvent::ON_LANGUAGE_SWITCH, myHandleLanguageSwitch) ) {
            _myI18nItem->removeEventListener(I18nEvent::ON_LANGUAGE_SWITCH, myHandleLanguageSwitch);
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
        I18nShapeWidgetPtr ptr = boost::static_pointer_cast<I18nShapeWidget>(shared_from_this());        
        EventCallbackPtr myHandleLanguageSwitch = EventCallbackPtr(new I18nWidgetEventCallback(ptr, &I18nShapeWidget::handleI18nOnLanguageSwitch));
        
        if (_myI18nItem) {
            if (_myI18nItem->hasEventListener(I18nEvent::ON_LANGUAGE_SWITCH, myHandleLanguageSwitch)) {
                _myI18nItem->removeEventListener(I18nEvent::ON_LANGUAGE_SWITCH, myHandleLanguageSwitch);
            }
            _myI18nItem = I18nItemPtr();
        }
        if (!i18nId_.empty()) {
            _myI18nItem = getI18nItemByName(i18nId_);
            if (!_myI18nItem) {
                throw I18nItemNotFoundException("no i18n item named " + i18nId_, PLUS_FILE_LINE);
            }
            _myI18nItem->addEventListener(I18nEvent::ON_LANGUAGE_SWITCH, myHandleLanguageSwitch);
            myHandleLanguageSwitch->execute(spark::EventPtr());
        } else {
            data_ = "";
        }
        _myDirtyFlag = true;
    }
    

}
