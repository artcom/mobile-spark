#include "Component.h"
#include "Event.h"

using namespace masl;

namespace spark {

    EventPtr createStageEvent(const masl::XMLNodePtr theXMLNode) {
        return EventPtr(new StageEvent(theXMLNode));
    }

    EventPtr createTouchEvent(const masl::XMLNodePtr theXMLNode) {
        return EventPtr(new TouchEvent(theXMLNode));
    }

    Event::Event(const std::string & theType, ComponentPtr theTarget) : type_(theType),target_(theTarget) {
    }
    Event::Event(const masl::XMLNodePtr theXMLNode) {
        type_ = theXMLNode->getStringValue("type", theXMLNode->nodeName);
    }

    Event::~Event() {
    }


    void 
    Event::operator () () {
        if (target_) {
            target_->dispatchEvent(shared_from_this());
        }
    }
    void
    Event::startDispatch() {
        dispatching_ = true;
        currentPhase_  = Event::IDLE;
    };

    void
    Event::cancelDispatch() {
        if (cancelable_) {
            finishDispatch();
        }
    };

    void
    Event::finishDispatch() {
        dispatching_ = false;
        currentPhase_ = Event::IDLE;
    };

    void
    Event::dispatchTo(ComponentPtr theCurrentTarget, EventPhase theCurrentPhase) {
        currentTarget_ = theCurrentTarget;
        currentPhase_  = theCurrentPhase;
    };

    void
    Event::dispatch() {
        currentTarget_ = target_;
        currentPhase_  = Event::TARGET;
    };


    StageEvent::StageEvent(const std::string & theType, ComponentPtr theTarget, const UInt64 theCurrentTime) :
        Event(theType, theTarget), currenttime_(theCurrentTime) 
    {}
    StageEvent::StageEvent(const masl::XMLNodePtr theXMLNode) :
        Event(theXMLNode)
    {
        currenttime_ = as<UInt64>(theXMLNode->getStringValue("time", "-1")); 
    }
    StageEvent::~StageEvent() {}

    TouchEvent::TouchEvent(const std::string & theType, ComponentPtr theTarget, const unsigned int theX, const unsigned int theY) :
        Event(theType, theTarget), x_(theX), y_(theY) 
    {}
    TouchEvent::TouchEvent(const masl::XMLNodePtr theXMLNode) :
        Event(theXMLNode),
        x_(as<unsigned int>(theXMLNode->attributes["x"])), 
        y_(as<unsigned int>(theXMLNode->attributes["y"]))
    {}

    TouchEvent::~TouchEvent() {}
}
