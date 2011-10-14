#include "EventDispatcher.h"

#include <masl/Logger.h>

#include "Component.h"

#include <list>

using namespace masl;

namespace spark {

    DEFINE_EXCEPTION(EventDispatcherException, masl::Exception);

    EventDispatcher::EventDispatcher() {
    }

    EventDispatcher::~EventDispatcher() {
    }

    bool 
    EventDispatcher::hasEventListener(const std::string & theType, const EventCallbackPtr theListener, const bool theUseCapture) const {
        std::pair<std::string, bool> myKey(theType, theUseCapture);
        std::pair<EventListenerMap::const_iterator, EventListenerMap::const_iterator> itp = _myListenersMap.equal_range(myKey);
        for (EventListenerMap::const_iterator mapIt = itp.first; mapIt != itp.second; ++mapIt) {
            if ((*mapIt).second == theListener) {
                return true;
            }
        }
        return false;
    }
    
    void
    EventDispatcher::addEventListener(const std::string & theType, const EventCallbackPtr theListener, const bool theUseCapture) {
        AC_INFO << "addEventListener for type " << theType << " capturing: " << theUseCapture;
        std::pair<std::string, bool> myKey(theType, theUseCapture);
        _myListenersMap.insert(std::pair<std::pair<std::string, bool>, EventCallbackPtr > (myKey, theListener));
    };

    void
    EventDispatcher::removeEventListener(const std::string & theType, const EventCallbackPtr theListener, const bool theUseCapture) {
        AC_INFO << "removeEventListener for type " << theType << " capturing: " << theUseCapture;
        std::pair<std::string, bool> myKey(theType, theUseCapture);
        std::pair<EventListenerMap::iterator, EventListenerMap::iterator> itp = _myListenersMap.equal_range(myKey);
        for (EventListenerMap::iterator mapIt = itp.first; mapIt != itp.second; ++mapIt) {
            if ((*mapIt).second == theListener) {
                AC_DEBUG << "remove listener: " << (*mapIt).second;
                _myListenersMap.erase(mapIt);
                return;
            }
        }
        throw EventDispatcherException(std::string("removeEventListener: no eventlistener found for " + theType + " capturing " + ((theUseCapture) ? "true" : "false")), PLUS_FILE_LINE);
    }

    void
    EventDispatcher::dispatchEvent(EventPtr theEvent) const {
        theEvent->startDispatch();

        ComponentPtr myCurrent = theEvent->getTarget();
        AC_TRACE << " dispatchEvent " << *theEvent;
        // collect dispatchers to capture on
        std::list<ComponentPtr> myCaptureList;
        while (myCurrent->getParent().lock()) {
            myCurrent = myCurrent->getParent().lock();
            myCaptureList.push_back(myCurrent);
        }
        // capture phase
        EventListenerKey myCaptureKey(theEvent->getType(), true);
        for (std::list<ComponentPtr>::const_iterator it = myCaptureList.begin(); it != myCaptureList.end(); ++it) {
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

        EventListenerKey myKey(theEvent->getType(), false);

        std::pair<EventListenerMap::const_iterator, EventListenerMap::const_iterator> itp = _myListenersMap.equal_range(myKey);
        for (EventListenerMap::const_iterator it = itp.first; it != itp.second; ++it) {
            AC_TRACE << "TARGET_PHASE: calling listener: "<<(*it).second;
            (*(*it).second)(theEvent);
            if (!theEvent->isDispatching()) {
                return;
            }
        }
        theEvent->finishDispatch();
    }

}
