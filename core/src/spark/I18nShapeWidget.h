// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_I18nShapeWidget_
#define _included_mobile_spark_I18nShapeWidget_

#include "ShapeWidget.h"

namespace spark {

    class I18nShapeWidget : public ShapeWidget{
        public:
            I18nShapeWidget(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~I18nShapeWidget() = 0;
            virtual void realize();
            void handleI18nOnLanguageSwitch(const EventPtr theEvent);

            void setI18nId(const std::string & theNewI18nId);
            void setI18nData(const std::string & theData);
            inline const std::string & getData() const {return _data;};
            virtual std::string getAttributesAsString() const;
        protected:
        private:            
            void attachToI18nItem();
            std::string _data;
            std::string _i18nId;
            EventCallbackPtr _myHandleLanguageSwitch;
    };

    typedef masl::Ptr<I18nShapeWidget> I18nShapeWidgetPtr;
    typedef masl::WeakPtr<I18nShapeWidget> I18nShapeWidgetWeakPtr;
    typedef MemberFunctionEventCallback<I18nShapeWidget, I18nShapeWidgetWeakPtr> I18nWidgetEventCallback;
    typedef masl::Ptr<I18nWidgetEventCallback > I18nWidgetEventCallbackPtr;
};

#endif
