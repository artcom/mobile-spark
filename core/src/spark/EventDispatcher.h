// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_spark_EventDispatcher_h_included_
#define _ac_mobile_spark_EventDispatcher_h_included_

#include <map>
#include <masl/Exception.h>
#include "Event.h"


namespace spark {

    DECLARE_EXCEPTION(EventDispatcherException, masl::Exception);

    typedef std::pair<const std::string, const Event::EventPhase> EventListenerKey;
    typedef std::multimap<EventListenerKey, const EventCallbackPtr > EventListenerMap;

    class EventDispatcher {
        public:

            EventDispatcher();
            ~EventDispatcher();

            bool hasEventListener(const std::string & theType, const EventCallbackPtr theListener, const Event::EventPhase thePhase = Event::TARGET) const;
            void addEventListener(const std::string & theType, const EventCallbackPtr theListener, const Event::EventPhase thePhase = Event::TARGET);
            void removeEventListener(const std::string & theType, const EventCallbackPtr theListener, const Event::EventPhase thePhase = Event::TARGET);
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
