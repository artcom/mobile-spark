// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "EventDispatcher.h"

#include <masl/Logger.h>

#include "Component.h"

#include <deque>

using namespace masl;

namespace spark {

    DEFINE_EXCEPTION(EventDispatcherException, masl::Exception);

    EventDispatcher::EventDispatcher() {
    }

    EventDispatcher::~EventDispatcher() {
    }

    bool 
    EventDispatcher::hasEventListener(const std::string & theType, const EventCallbackPtr theListener, const Event::EventPhase thePhase) const {
        std::pair<std::string, Event::EventPhase> myKey(theType, thePhase);
        std::pair<EventListenerMap::const_iterator, EventListenerMap::const_iterator> itp = _myListenersMap.equal_range(myKey);
        for (EventListenerMap::const_iterator mapIt = itp.first; mapIt != itp.second; ++mapIt) {
            if ((*mapIt).second == theListener) {
                return true;
            }
        }
        return false;
    }
    
    void
    EventDispatcher::addEventListener(const std::string & theType, const EventCallbackPtr theListener, const Event::EventPhase thePhase) {
        AC_INFO << "addEventListener for type " << theType << " capturing: " << (thePhase == Event::CAPTURING) << " bubbling: " << (thePhase == Event::BUBBLING);
        std::pair<std::string, Event::EventPhase> myKey(theType, thePhase);
        _myListenersMap.insert(std::pair<std::pair<std::string, Event::EventPhase>, EventCallbackPtr > (myKey, theListener));
    };

    void
    EventDispatcher::removeEventListener(const std::string & theType, const EventCallbackPtr theListener, const Event::EventPhase thePhase) {
        AC_INFO << "removeEventListener for type " << theType << " capturing: " << (thePhase == Event::CAPTURING) << " bubbling: " << (thePhase == Event::BUBBLING);
        std::pair<std::string, Event::EventPhase> myKey(theType, thePhase);
        std::pair<EventListenerMap::iterator, EventListenerMap::iterator> itp = _myListenersMap.equal_range(myKey);
        for (EventListenerMap::iterator mapIt = itp.first; mapIt != itp.second; ++mapIt) {
            if ((*mapIt).second == theListener) {
                AC_DEBUG << "remove listener: " << (*mapIt).second;
                _myListenersMap.erase(mapIt);
                return;
            }
        }
        throw EventDispatcherException(std::string("removeEventListener: no eventlistener found for " + theType + " capturing: " + masl::as_string(thePhase == Event::CAPTURING) + " bubbling: " + masl::as_string(thePhase == Event::BUBBLING)), PLUS_FILE_LINE);
    }

    void
    EventDispatcher::dispatchEvent(EventPtr theEvent) const {
        theEvent->startDispatch();

        ComponentPtr myCurrent = theEvent->getTarget();
        AC_TRACE << " dispatchEvent " << *theEvent;

        // collect dispatchers to capture & bubble on
        std::deque<ComponentPtr> myTopToBottomList;
        myTopToBottomList.push_front(myCurrent);
        while (myCurrent->getParent().lock()) {
            myCurrent = myCurrent->getParent().lock();
            myTopToBottomList.push_front(myCurrent);
        }
        
        // capture phase
        EventListenerKey myCaptureKey(theEvent->getType(), Event::CAPTURING);
        for (std::deque<ComponentPtr>::const_iterator it = myTopToBottomList.begin(); it != myTopToBottomList.end(); ++it) {
            if ((*it)->hasEventListenersForType(myCaptureKey)) {
                theEvent->dispatchTo(*it, Event::CAPTURING);
                EventListenerMap myListeners = (*it)->getEventListeners();
                std::pair<EventListenerMap::const_iterator, EventListenerMap::const_iterator> itp = myListeners.equal_range(myCaptureKey);
                for (EventListenerMap::const_iterator mapIt = itp.first; mapIt != itp.second; ++mapIt) {
                    AC_TRACE << "CAPTURE_PHASE: calling listener: "<<(*mapIt).second;
                    (*(*mapIt).second)(theEvent);
                    if (!theEvent->isDispatching()) {
                        return;
                    }
                }
            }
        }

        // target phase
        theEvent->dispatch();

        EventListenerKey myKey(theEvent->getType(), Event::TARGET);

        std::pair<EventListenerMap::const_iterator, EventListenerMap::const_iterator> itp = _myListenersMap.equal_range(myKey);
        for (EventListenerMap::const_iterator it = itp.first; it != itp.second; ++it) {
            AC_TRACE << "TARGET_PHASE: calling listener: "<<(*it).second;
            (*(*it).second)(theEvent);
            if (!theEvent->isDispatching()) {
                return;
            }
        }
        
        // bubble phase
        EventListenerKey myBubbleKey(theEvent->getType(), Event::BUBBLING);
        for (std::deque<ComponentPtr>::const_reverse_iterator it = myTopToBottomList.rbegin(); it != myTopToBottomList.rend(); ++it) {
            if ((*it)->hasEventListenersForType(myBubbleKey)) {
                theEvent->dispatchTo(*it, Event::BUBBLING);
                EventListenerMap myListeners = (*it)->getEventListeners();
                std::pair<EventListenerMap::const_iterator, EventListenerMap::const_iterator> itp = myListeners.equal_range(myBubbleKey);
                for (EventListenerMap::const_iterator mapIt = itp.first; mapIt != itp.second; ++mapIt) {
                    AC_TRACE << "BUBBLE_PHASE: calling listener: "<<(*mapIt).second;
                    (*(*mapIt).second)(theEvent);
                    if (!theEvent->isDispatching()) {
                        return;
                    }
                }
            }
        }

        theEvent->finishDispatch();
    }

}
