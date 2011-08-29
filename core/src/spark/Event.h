#ifndef _ac_mobile_spark_Event_h_included_
#define _ac_mobile_spark_Event_h_included_

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/enum.hpp>
#include <masl/Logger.h>
#include <masl/Callback.h>

namespace spark {

    class Component;
    typedef boost::shared_ptr<Component> ComponentPtr;

    class Event {
        public:

            Event(const std::string & theType, ComponentPtr theTarget);
            virtual ~Event() = 0;

            enum EventPhase {
                IDLE,
                CAPTURING,
                TARGET,
                BUBBLING
            };

            void cancelDispatch();
            void startDispatch();
            void finishDispatch();
            void dispatchTo(ComponentPtr theCurrentTarget, EventPhase theCurrentPhase);
            void dispatch();
            const std::string & getType() const {return type_;};
            const ComponentPtr & getTarget() const {return target_;};
            bool isDispatching() const {return dispatching_;};

        private:
            std::string type_;
            ComponentPtr target_;
            ComponentPtr currentTarget_;
            EventPhase currentPhase_;
            bool dispatching_;
            bool cancelable_;
    };

    typedef boost::shared_ptr<Event> EventPtr;

    class StageEvent : public Event {
        public:

            StageEvent(const std::string & theType, ComponentPtr theTarget, const long theCurrentTime);
            virtual ~StageEvent();

            long currenttime_;
    };

    typedef boost::shared_ptr<StageEvent> StageEventPtr;

    class TouchEvent : public Event {
        public:

            TouchEvent(const std::string & theType, ComponentPtr theTarget, const unsigned int theX, const unsigned int theY);
            virtual ~TouchEvent();

            unsigned int getX() const { return x_;};
            unsigned int getY() const { return y_;};

        private:
            unsigned int x_;
            unsigned int y_;
    };

    typedef boost::shared_ptr<TouchEvent> TouchEventPtr;

    class EventCallback {
    public:
        EventCallback(ComponentPtr theObject, void (Component::*theFunctionPtr)(EventPtr)): 
            _myObjectPtr(theObject),
            _myFunctionPointer(theFunctionPtr) {
        };
        virtual ~EventCallback() {};

        void execute(EventPtr theEvent) const {
            (_myObjectPtr.get()->*_myFunctionPointer)(theEvent);
        };
        virtual void operator() (EventPtr theEvent) {execute(theEvent);}
    private:
        ComponentPtr _myObjectPtr;
        void (Component::*_myFunctionPointer)(EventPtr);
    };
    typedef boost::shared_ptr<EventCallback> EventCallbackPtr;

}
#endif
