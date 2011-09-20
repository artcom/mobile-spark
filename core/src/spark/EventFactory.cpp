#include "EventFactory.h"


#include <masl/Logger.h>
#include <masl/XMLNode.h>

using namespace masl;

namespace spark {

    //TODO: decentralize this again
    void EventFactory::setupFactory() {
        bool registered = registerEvent(StageEvent::CLASSNAME, spark::createEvent<StageEvent>);
        registered = registerEvent(WindowEvent::CLASSNAME, spark::createEvent<WindowEvent>);
        registered = registerEvent(TouchEvent::CLASSNAME, spark::createEvent<TouchEvent>);
        registered = registerEvent(GestureEvent::CLASSNAME, spark::createEvent<GestureEvent>);
        registered = registerEvent(SensorEvent::CLASSNAME, spark::createEvent<SensorEvent>);
        registered = registerEvent(I18nEvent::CLASSNAME, spark::createEvent<I18nEvent>);
    };

    EventFactory::EventFactory() {
        setupFactory();
    }
    EventFactory::~EventFactory() {}

    // Returns 'true' if registration was successful
    bool EventFactory::registerEvent(const std::string & theEventName,
                    const CreateEventCallback theCreateFn) {
        return _myCreateCallbackMap.insert(CallbackMap::value_type(theEventName, theCreateFn)).second;
    }

    // Returns 'true' if the event name was registered before
    bool EventFactory::unregisterEvent(const std::string & theEventName) {
        return _myCreateCallbackMap.erase(theEventName) == 1;
    }

	EventPtr EventFactory::handleEvent(const std::string & theEventString) const {
        AC_PRINT << "handle event : " << theEventString;
        XMLNodePtr myXMLNode(new XMLNode(theEventString));
        CallbackMap::const_iterator i = _myCreateCallbackMap.find(myXMLNode->nodeName);
        if (i == _myCreateCallbackMap.end()) {
            throw EventFactoryException("Unknown Event: " + myXMLNode->nodeName, PLUS_FILE_LINE);
        }
        return (i->second)(myXMLNode);
    }

}
