#ifndef _included_mobile_spark_I18nHandler_
#define _included_mobile_spark_I18nHandler_

#include <boost/smart_ptr/shared_ptr.hpp>
#include <masl/XMLNode.h>
#include "Event.h"

namespace spark {

    class Widget;
    typedef boost::shared_ptr<Widget> WidgetPtr;
    class I18nContext;

    class I18nHandler : public boost::enable_shared_from_this<I18nHandler> {
    public:
        I18nHandler(const masl::XMLNodePtr theXMLNode, const std::string & theKey);
        ~I18nHandler();
        void realize(const WidgetPtr theWidget);

        std::string data_;
        std::string i18nId_;
    private:
        WidgetPtr widget_;

        void attachToI18nItem();
        void handleI18nOnLanguageSwitch(const EventPtr theEvent = EventPtr());
    };
    typedef boost::shared_ptr<I18nHandler> I18nHandlerPtr;
    typedef MemberFunctionEventCallback<I18nHandler, I18nHandlerPtr> I18nHandlerCB;
};
#endif

