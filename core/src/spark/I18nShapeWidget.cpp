// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "I18nContext.h"
#include "I18nShapeWidget.h"

namespace spark {
    I18nShapeWidget::I18nShapeWidget(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
        : ShapeWidget(theApp, theXMLNode), 
        _i18nId(theXMLNode->getAttributeAs<std::string>("i18nId", ""))    {
    }

    I18nShapeWidget::~I18nShapeWidget() {
        AC_INFO << "delete I18nShapeWidget " << getName();
        if (_myI18nItem && _myI18nItem->hasEventListener(I18nEvent::ON_LANGUAGE_SWITCH, _myHandleLanguageSwitch) ) {
            _myI18nItem->removeEventListener(I18nEvent::ON_LANGUAGE_SWITCH, _myHandleLanguageSwitch);
        }
    }
    
    void 
    I18nShapeWidget::setI18nData(const std::string & theData) {
        _data = theData;
    }
    
    void 
    I18nShapeWidget::setI18nId(const std::string & theNewI18nId) {
        _i18nId = theNewI18nId;
        attachToI18nItem();
    }

    void
    I18nShapeWidget::realize()  {
        ShapeWidget::realize();
        I18nShapeWidgetPtr ptr = boost::static_pointer_cast<I18nShapeWidget>(shared_from_this());        
        _myHandleLanguageSwitch = EventCallbackPtr(new I18nWidgetEventCallback(ptr, &I18nShapeWidget::handleI18nOnLanguageSwitch));
        if (!_i18nId.empty()) {
            attachToI18nItem();
        }            
    }

    void 
    I18nShapeWidget::handleI18nOnLanguageSwitch(const EventPtr theEvent) {
        if (getI18nItem()) {
            setI18nData(getI18nItem()->getLanguageData());
            _myDirtyFlag = true;
            AC_DEBUG << "on language switch data " << _data;
        }
    }

    void
    I18nShapeWidget::attachToI18nItem() {
        if (_myI18nItem) {
            if (_myI18nItem->hasEventListener(I18nEvent::ON_LANGUAGE_SWITCH, _myHandleLanguageSwitch)) {
                _myI18nItem->removeEventListener(I18nEvent::ON_LANGUAGE_SWITCH, _myHandleLanguageSwitch);
            }
            _myI18nItem = I18nItemPtr();
        }
        if (!_i18nId.empty()) {
            _myI18nItem = getI18nItemByName(_i18nId);
            if (!_myI18nItem) {
                throw I18nItemNotFoundException("no i18n item named " + _i18nId, PLUS_FILE_LINE);
            }
            _myI18nItem->addEventListener(I18nEvent::ON_LANGUAGE_SWITCH, _myHandleLanguageSwitch);
            _myHandleLanguageSwitch->execute(spark::EventPtr());
        } else {
            _data = "";
        }
        _myDirtyFlag = true;
    }

    std::string 
    I18nShapeWidget::getAttributesAsString() const {
        return ShapeWidget::getAttributesAsString() + " i18nId=\""+_i18nId+"\"";
    }
}
