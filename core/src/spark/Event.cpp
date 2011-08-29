#include "Event.h"

namespace spark {

    Event::Event(const std::string & theType, ComponentPtr theTarget) : type_(theType),target_(theTarget) {
    }

    Event::~Event() {}

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


    StageEvent::StageEvent(const std::string & theType, ComponentPtr theTarget, const long theCurrentTime) :
        Event(theType, theTarget), currenttime_(theCurrentTime) 
    {}
    StageEvent::~StageEvent() {}

    TouchEvent::TouchEvent(const std::string & theType, ComponentPtr theTarget, const unsigned int theX, const unsigned int theY) :
        Event(theType, theTarget), x_(theX), y_(theY) 
    {}
    TouchEvent::~TouchEvent() {}
}
