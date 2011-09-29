#ifndef _included_mobile_spark_I18nHandler_
#define _included_mobile_spark_I18nHandler_

#include "Event.h"

namespace masl {
    class XMLNode;
    typedef masl::Ptr<XMLNode> XMLNodePtr;
};

namespace spark {

    class Widget;
    typedef masl::Ptr<Widget> WidgetPtr;
    class I18nContext;

    class I18nHandler {
    public:
        I18nHandler();
        void setI18nId(std::string theNewI18nId);
        void setI18nData(std::string theData);
        
        virtual ~I18nHandler();
        void realize(const WidgetPtr theWidget, EventCallbackPtr theHandleLanguageSwitch);

        std::string data_;
        std::string i18nId_;
    private:
        EventCallbackPtr _myHandleI18nEventCB;
        WidgetPtr widget_;

        void attachToI18nItem();
    };
    typedef masl::Ptr<I18nHandler> I18nHandlerPtr;
    typedef MemberFunctionEventCallback<I18nHandler, I18nHandlerPtr> I18nHandlerCB;
};
#endif

