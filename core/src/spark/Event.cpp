#include "Component.h"
#include "Event.h"

using namespace masl;

namespace spark {
    const char * const StageEvent::FRAME = "frame";
    const char * const TouchEvent::TAP = "tap";
    const char * const TouchEvent::DOUBLETAP = "doubletap";
    const char * const TouchEvent::LONGPRESS = "longpress";
    const char * const TouchEvent::PICKED = "picked";
    const char * const GestureEvent::PAN = "pan";
    const char * const GestureEvent::PINCH = "pinch";
    const char * const GestureEvent::ROTATE = "rotate";
    const char * const GestureEvent::SWIPE_LEFT = "swipe-left";
    const char * const GestureEvent::SWIPE_RIGHT = "swipe-right";

    Event::Event(const std::string & theType, ComponentPtr theTarget) : type_(theType),target_(theTarget) {
    }
    Event::Event(const masl::XMLNodePtr theXMLNode)
         : type_(theXMLNode->getAttributeAs<std::string>("type", theXMLNode->nodeName)) {
        
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
        currenttime_(theXMLNode->getAttributeAs<UInt64>("time", 0))
    {}
    StageEvent::~StageEvent() {}

    TouchEvent::TouchEvent(const std::string & theType, ComponentPtr theTarget, const unsigned int theX, const unsigned int theY) :
        Event(theType, theTarget), x_(theX), y_(theY) 
    {}
    TouchEvent::TouchEvent(const masl::XMLNodePtr theXMLNode) :
        Event(theXMLNode),
        x_(theXMLNode->getAttributeAs<unsigned int>("x", 0)), 
        y_(theXMLNode->getAttributeAs<unsigned int>("y", 0))
    {}

    TouchEvent::~TouchEvent() {}




    GestureEvent::GestureEvent(const std::string & theType, ComponentPtr theTarget, const unsigned int theX, const unsigned int theY, const int dx, const int dy)
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
           x_(theXMLNode->getAttributeAs<unsigned int>("x", 0)), 
           y_(theXMLNode->getAttributeAs<unsigned int>("y", 0)),
           dx_(theXMLNode->getAttributeAs<int>("dx", 0)), 
           dy_(theXMLNode->getAttributeAs<int>("dy", 0)),
           factor_(theXMLNode->getAttributeAs<float>("factor", 0.0)),
           direction_(theXMLNode->getAttributeAs<std::string>("direction", ""))
    {}
    
    GestureEvent::~GestureEvent() {}
}
