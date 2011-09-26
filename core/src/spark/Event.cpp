#include "Event.h"

#include <masl/Logger.h>

#include "Component.h"

using namespace masl;

namespace spark {
    const char * const TouchEvent::CLASSNAME = "TouchEvent";
    const char * const StageEvent::CLASSNAME = "StageEvent";
    const char * const WindowEvent::CLASSNAME = "WindowEvent";
    const char * const GestureEvent::CLASSNAME = "GestureEvent";
    const char * const SensorEvent::CLASSNAME = "SensorEvent";
    const char * const I18nEvent::CLASSNAME = "I18nEvent";

    const char * const StageEvent::FRAME = "frame";
    const char * const StageEvent::PAUSE = "pause";
    const char * const WindowEvent::ON_RESIZE = "on_resize";
    const char * const I18nEvent::ON_LANGUAGE_SWITCH = "on_language_switch";
        
    const char * const TouchEvent::TAP = "tap";
    const char * const TouchEvent::DOUBLETAP = "doubletap";
    const char * const TouchEvent::LONGPRESS = "longpress";
    const char * const TouchEvent::PICKED = "picked";
    const char * const TouchEvent::DOWN = "down";
    const char * const TouchEvent::UP = "up";

    const char * const GestureEvent::PAN = "pan";
    const char * const GestureEvent::PINCH = "pinch";
    const char * const GestureEvent::ROTATE = "rotate";
    const char * const GestureEvent::SWIPE_LEFT = "swipe-left";
    const char * const GestureEvent::SWIPE_RIGHT = "swipe-right";
    const char * const SensorEvent::ACCELEROMETER = "ACCELEROMETER";
	const char * const SensorEvent::GRAVITY = "GRAVITY";
	const char * const SensorEvent::GYROSCOPE = "GYROSCOPE";
	const char * const SensorEvent::LIGHT = "LIGHT";
	const char * const SensorEvent::LINEAR_ACCELERATION = "LINEAR_ACCELERATION";
	const char * const SensorEvent::MAGNETIC_FIELD = "MAGNETIC_FIELD";
	const char * const SensorEvent::ORIENTATION = "ORIENTATION";
	const char * const SensorEvent::PRESSURE = "PRESSURE";
	const char * const SensorEvent::PROXIMITY = "PROXIMITY";
	const char * const SensorEvent::ROTATION_VECTOR = "ROTATION_VECTOR";
	const char * const SensorEvent::TEMPERATURE = "TEMPERATURE";


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

    std::ostream &
    Event::print(std::ostream & os) const {
        os << classname_() << " type: '" << type_ << "' target: '" << *target_ << "' currentPhase: " << currentPhase_;
        return os;
    }

    void
    Event::startDispatch() {
        dispatching_ = true;
        currentPhase_  = Event::IDLE;
        currentTarget_ = ComponentPtr();
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
        currentTarget_ = ComponentPtr();
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

    WindowEvent::WindowEvent(const std::string & theType, ComponentPtr theTarget,
                             const unsigned int theNewWidth, const unsigned int theNewHeight, const unsigned int theOldWidth, const unsigned int theOldHeight) :
        Event(theType, theTarget),
        size_(theNewWidth, theNewHeight),
        oldsize_(theOldWidth, theOldHeight)
    {}
    
    WindowEvent::WindowEvent(const masl::XMLNodePtr theXMLNode) :
        Event(theXMLNode),
        size_(theXMLNode->getAttributeAs<vector2>("newsize", vector2(0,0))),
        oldsize_(theXMLNode->getAttributeAs<vector2>("oldsize", vector2(0,0)))
    {}

    WindowEvent::~WindowEvent() {}



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


    SensorEvent::SensorEvent(const std::string & theType, ComponentPtr theTarget, const float theValue0, const float theValue1, const float theValue2)
         : Event(theType, theTarget), value0_(theValue0), value1_(theValue1), value2_(theValue2)
    {}
    SensorEvent::SensorEvent(const masl::XMLNodePtr theXMLNode) :
        Event(theXMLNode),
        value0_(theXMLNode->getAttributeAs<float>("value0", 0.0)),
        value1_(theXMLNode->getAttributeAs<float>("value1", 0.0)),
        value2_(theXMLNode->getAttributeAs<float>("value2", 0.0))
    {}
    SensorEvent::~SensorEvent() {}


    I18nEvent::I18nEvent(const masl::XMLNodePtr theXMLNode) :
        Event(theXMLNode) {
    }
    I18nEvent::I18nEvent(const std::string & theType, ComponentPtr theTarget)
        : Event(theType, theTarget) {
    }
    I18nEvent::~I18nEvent() {}
}
