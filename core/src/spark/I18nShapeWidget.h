#ifndef _included_mobile_spark_I18nShapeWidget_
#define _included_mobile_spark_I18nShapeWidget_

#include "Widget.h"
#include "ShapeWidget.h"
#include "I18nHandler.h"
#include <mar/Shape.h>

namespace spark {

    class I18nShapeWidget : public ShapeWidget, public I18nHandler {
        public:
            I18nShapeWidget(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~I18nShapeWidget() = 0;
            virtual void realize();
            void handleI18nOnLanguageSwitch(const EventPtr theEvent = EventPtr());
            void setI18nItem(I18nItemPtr theItem) {_myI18nItem = theItem;}
        protected:
        private:
            mar::ShapePtr _myShape;
    };
    typedef masl::Ptr<I18nShapeWidget> I18nShapeWidgetPtr;
    typedef MemberFunctionEventCallback<I18nShapeWidget, I18nShapeWidgetPtr> I18nWidgetEventCallback;
    typedef masl::Ptr<MemberFunctionEventCallback<I18nShapeWidget, I18nShapeWidgetPtr> > I18nWidgetEventCallbackPtr;
        
};

#endif
