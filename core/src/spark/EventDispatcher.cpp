#include <list>
#include "Component.h"
#include "EventDispatcher.h"

using namespace masl;

namespace spark {

    EventDispatcher::EventDispatcher() {
    }

    EventDispatcher::~EventDispatcher() {
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
        if (itp.first != itp.second) {
            _myListenersMap.erase(itp.first, itp.second);
        } else {
            throw EventDispatcherException(std::string("removeEventListener: no eventlistener found for " + theType), PLUS_FILE_LINE);
        }
    }

    void
    EventDispatcher::dispatchEvent(EventPtr theEvent) const {
        theEvent->startDispatch();

        ComponentPtr myCurrent = theEvent->getTarget();
        AC_TRACE << " dispatchEvent " << *theEvent;
        // collect dispatchers to capture on
        std::list<ComponentPtr> myCaptureList;
        while (myCurrent->getParent()) {
            myCurrent = myCurrent->getParent();
            myCaptureList.push_front(myCurrent);
        }

        // capture phase
        EventListenerKey myCaptureKey(theEvent->getType(), true);
        for (std::list<ComponentPtr>::iterator it = myCaptureList.begin(); it != myCaptureList.end(); ++it) {
            if ((*it)->hasEventListener(myCaptureKey)) {
                theEvent->dispatchTo(*it, Event::CAPTURING);
                EventListenerMap myListeners = (*it)->getEventListeners();
                std::pair<EventListenerMap::const_iterator, EventListenerMap::const_iterator> itp = myListeners.equal_range(myCaptureKey);
                for (EventListenerMap::const_iterator mapIt = itp.first; mapIt != itp.second; ++mapIt) {
                    AC_TRACE << "CAPTURE_PHASE: callling listener: "<<(*mapIt).second;
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

    inline bool
    EventDispatcher::hasEventListener(const EventListenerKey & theKey ) const {
        return (_myListenersMap.find(theKey) != _myListenersMap.end());
    }
}
