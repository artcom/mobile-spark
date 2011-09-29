#ifndef _ac_mobile_spark_EventDispatcher_h_included_
#define _ac_mobile_spark_EventDispatcher_h_included_

#include <map>
#include <masl/Exception.h>
#include "Event.h"


namespace spark {

    DECLARE_EXCEPTION(EventDispatcherException, masl::Exception);

    typedef std::pair<const std::string, const bool> EventListenerKey;
    typedef std::multimap<EventListenerKey, const EventCallbackPtr > EventListenerMap;

    class EventDispatcher {
        public:

            EventDispatcher();
            ~EventDispatcher();

            bool hasEventListener(const std::string & theType, const EventCallbackPtr theListener, const bool theUseCapture = false) const;
            void addEventListener(const std::string & theType, const EventCallbackPtr theListener, const bool theUseCapture = false);
            void removeEventListener(const std::string & theType, const EventCallbackPtr theListener, const bool theUseCapture = false);
            void dispatchEvent(const EventPtr theEvent) const;
            const EventListenerMap & getEventListeners() const {return _myListenersMap;};
        private:
            inline bool hasEventListenersForType(const EventListenerKey & theKey ) const {
                return (_myListenersMap.find(theKey) != _myListenersMap.end());
            }

            EventListenerMap _myListenersMap;
    };
}
#endif
