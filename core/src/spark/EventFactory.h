#ifndef _included_mobile_spark_EventFactory_
#define _included_mobile_spark_EventFactory_

#include <masl/Singleton.h>
#include <masl/Exception.h>
#include "BaseApp.h"

#include "Event.h"

namespace spark {

    DEFINE_EXCEPTION(EventFactoryException, Exception);

    class EventFactory : public masl::Singleton<EventFactory> {
    public:
        EventFactory();
        virtual ~EventFactory();
        typedef EventPtr (*CreateEventCallback)( const XMLNodePtr theXMLNode);
    private:
        typedef std::map<const std::string, CreateEventCallback> CallbackMap;
    public:
        bool registerEvent(const std::string & theEventName,
                        const CreateEventCallback theCreateFn);
        bool unregisterEvent(const std::string & theEventName);
        EventPtr handleEvent(const std::string & theEvent) const;

    private:
        CallbackMap _myCreateCallbackMap;
        void setupFactory();
    };
};
#endif 