#ifndef _ac_mobile_spark_EventDispatcher_h_included_
#define _ac_mobile_spark_EventDispatcher_h_included_

#include <map>
#include <boost/smart_ptr/shared_ptr.hpp>
//#include <boost/enable_shared_from_this.hpp>
//#include <masl/Callback.h>
#include <masl/Logger.h>
#include <masl/Exception.h>
#include "Event.h"


namespace spark {

    DEFINE_EXCEPTION(EventDispatcherException, masl::Exception);

    typedef std::pair<const std::string, const bool> EventListenerKey;
    typedef std::multimap<EventListenerKey, const EventCallbackPtr > EventListenerMap;

    class Event;
    typedef boost::shared_ptr<Event> EventPtr;

    class EventDispatcher {
        public:

            EventDispatcher();
            ~EventDispatcher();

            void addEventListener(const std::string & theType, const EventCallbackPtr theListener, const bool theUseCapture = false);
            void removeEventListener(const std::string & theType, const EventCallbackPtr theListener, const bool theUseCapture = false);
            void dispatchEvent(const EventPtr theEvent) const;
            const EventListenerMap & getEventListeners() const {return _myListenersMap;};
            inline bool hasEventListener(const EventListenerKey & theKey ) const;
        private:

            EventListenerMap _myListenersMap;
    };
}
#endif
