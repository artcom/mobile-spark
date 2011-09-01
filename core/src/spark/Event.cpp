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

    EventPtr createGestureEvent(const masl::XMLNodePtr theXMLNode) {
        return EventPtr(new GestureEvent(theXMLNode));
    }

    Event::Event(const std::string & theType, ComponentPtr theTarget) : type_(theType),target_(theTarget) {
    }
    Event::Event(const masl::XMLNodePtr theXMLNode)
         : type_(theXMLNode->getStringValue("type", theXMLNode->nodeName)) 
    {
        
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
        Event(theXMLNode),
        currenttime_(as<UInt64>(theXMLNode->getStringValue("time", "0")))
    {}
    StageEvent::~StageEvent() {}

    TouchEvent::TouchEvent(const std::string & theType, ComponentPtr theTarget, const unsigned int theX, const unsigned int theY) :
        Event(theType, theTarget), x_(theX), y_(theY) 
    {}
    TouchEvent::TouchEvent(const masl::XMLNodePtr theXMLNode) :
        Event(theXMLNode),
        x_(as<unsigned int>(theXMLNode->getStringValue("x", "0"))), 
        y_(as<unsigned int>(theXMLNode->getStringValue("y", "0")))
    {}

    TouchEvent::~TouchEvent() {}




    GestureEvent::GestureEvent(const std::string & theType, ComponentPtr theTarget, const int theX, const int theY, const int dx, const int dy)
         : Event(theType, theTarget), x_(theX), y_(theY), dx_(dx), dy_(dy)
    {}
    GestureEvent::GestureEvent(const std::string & theType, ComponentPtr theTarget, const float theFactor)
         : Event(theType, theTarget), factor_(theFactor)
    { }
    GestureEvent::GestureEvent(const std::string & theType, ComponentPtr theTarget, const std::string & theDirection)
         : Event(theType, theTarget), direction_(theDirection)
    { }
    GestureEvent::GestureEvent(const masl::XMLNodePtr theXMLNode)
         : Event(theXMLNode),
           x_(as<unsigned int>(theXMLNode->getStringValue("x", "0"))), 
           y_(as<unsigned int>(theXMLNode->getStringValue("y", "0"))),
           dx_(as<int>(theXMLNode->getStringValue("dx", "0"))),
           dy_(as<int>(theXMLNode->getStringValue("dy", "0"))),
           factor_(as<float>(theXMLNode->getStringValue("factor", "0.0"))),
           direction_(theXMLNode->getStringValue("direction", ""))
    {}
    
    GestureEvent::~GestureEvent() {}
}
