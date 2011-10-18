#ifndef _included_mobile_spark_I18nShapeWidget_
#define _included_mobile_spark_I18nShapeWidget_

#include "Widget.h"
#include "ShapeWidget.h"
#include <mar/Shape.h>

namespace spark {

    class I18nShapeWidget : public ShapeWidget{
        public:
            I18nShapeWidget(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~I18nShapeWidget() = 0;
            virtual void realize();
            void handleI18nOnLanguageSwitch(const EventPtr theEvent);

            void setI18nId(const std::string & theNewI18nId);
            void setI18nData(const std::string & theData);
        protected:
            std::string data_;
        private:            
            void attachToI18nItem();
            std::string i18nId_;
            EventCallbackPtr _myHandleLanguageSwitch;
    };

    typedef masl::Ptr<I18nShapeWidget> I18nShapeWidgetPtr;
    typedef masl::WeakPtr<I18nShapeWidget> I18nShapeWidgetWeakPtr;
    typedef MemberFunctionEventCallback<I18nShapeWidget, I18nShapeWidgetWeakPtr> I18nWidgetEventCallback;
    typedef masl::Ptr<I18nWidgetEventCallback > I18nWidgetEventCallbackPtr;
};

#endif
