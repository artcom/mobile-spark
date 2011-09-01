#ifndef _ac_mobile_spark_Event_h_included_
#define _ac_mobile_spark_Event_h_included_

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <masl/Settings.h>
#include <masl/Logger.h>
#include <masl/Callback.h>
#include <masl/XMLNode.h>

namespace spark {

    class Component;
    typedef boost::shared_ptr<Component> ComponentPtr;

    class Event : public boost::enable_shared_from_this<Event> {
        public:

            Event(const std::string & theType, ComponentPtr theTarget);
            Event(const masl::XMLNodePtr theXMLNode);
            virtual ~Event() = 0;

            enum EventPhase {
                IDLE,
                CAPTURING,
                TARGET
            };

            void operator () ();
            void cancelDispatch();
            void startDispatch();
            void finishDispatch();
            void dispatchTo(ComponentPtr theCurrentTarget, EventPhase theCurrentPhase);
            void dispatch();
            const std::string & getType() const {return type_;};
            const ComponentPtr & getTarget() const {return target_;};
            bool isDispatching() const {return dispatching_;};
            void connect(ComponentPtr theComponent) {target_ = theComponent;};

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

            StageEvent(const std::string & theType, ComponentPtr theTarget, const masl::UInt64 theCurrentTime);
            StageEvent(const masl::XMLNodePtr theXMLNode);
            virtual ~StageEvent();

            masl::UInt64 getCurrentTime() const { return currenttime_;};
            masl::UInt64 currenttime_;
    };

    typedef boost::shared_ptr<StageEvent> StageEventPtr;

    class TouchEvent : public Event {
        public:

            TouchEvent(const std::string & theType, ComponentPtr theTarget, const unsigned int theX, const unsigned int theY);
            TouchEvent(const masl::XMLNodePtr theXMLNode);
            virtual ~TouchEvent();

            unsigned int getX() const { return x_;};
            unsigned int getY() const { return y_;};

        private:
            unsigned int x_;
            unsigned int y_;
    };

    typedef boost::shared_ptr<TouchEvent> TouchEventPtr;

    class GestureEvent : public Event {
        public:

            GestureEvent(const std::string & theType, ComponentPtr theTarget, const int theX, const int theY, const int dx, const int dy);
            GestureEvent(const std::string & theType, ComponentPtr theTarget, const float theScale);
            GestureEvent(const std::string & theType, ComponentPtr theTarget, const float theAngle);
            GestureEvent(const std::string & theType, ComponentPtr theTarget, const std::string & theDirection);
            GestureEvent(const masl::XMLNodePtr theXMLNode);
            virtual ~GestureEvent();

            unsigned int getX() const { return x_;};
            unsigned int getY() const { return y_;};

        private:
            unsigned int x_;
            unsigned int y_;
            float scale_;
            float angle_;
            std::string direction_;
            int dy_;
            int dx_;
    };

    typedef boost::shared_ptr<TouchEvent> TouchEventPtr;
    EventPtr createStageEvent(const masl::XMLNodePtr theXMLNode);
    EventPtr createTouchEvent(const masl::XMLNodePtr theXMLNode);
    EventPtr createGestureEvent(const masl::XMLNodePtr theXMLNode);





    class EventCallback {
    public:
        virtual ~EventCallback() {};
        virtual void execute(EventPtr theEvent) const = 0;
        virtual void operator() (EventPtr theEvent) {execute(theEvent);};
    };

    typedef boost::shared_ptr<EventCallback> EventCallbackPtr;




    typedef void (*FreeFunctionEventPtr)(EventPtr);

    class FreeFunctionEventCallback : public EventCallback {
    public:
        FreeFunctionEventCallback(FreeFunctionEventPtr theFunctionPtr): 
            EventCallback(),
            _myFunctionPointer(theFunctionPtr) {
        };
        virtual ~FreeFunctionEventCallback() {};

        virtual void execute(EventPtr theEvent) const {
            _myFunctionPointer(theEvent);
        };
    private:
        FreeFunctionEventPtr _myFunctionPointer; 
    };
    typedef boost::shared_ptr<FreeFunctionEventCallback> FreeFunctionEventCallbackPtr;



    template < typename T, typename TP>
    class MemberFunctionEventCallback : public EventCallback {
    public:
        MemberFunctionEventCallback(TP theObject, void (T::*theFunctionPtr)(EventPtr)): 
            _myObjectPtr(theObject),
            _myFunctionPointer(theFunctionPtr) {
        };
        virtual ~MemberFunctionEventCallback() {};

        virtual void execute(EventPtr theEvent) const {
            (_myObjectPtr.get()->*_myFunctionPointer)(theEvent);
        };
    private:
        TP _myObjectPtr;
        void (T::*_myFunctionPointer)(EventPtr);
    };

}
#endif
