#ifndef _ac_mobile_spark_Event_h_included_
#define _ac_mobile_spark_Event_h_included_

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <masl/Settings.h>
#include <masl/Logger.h>
#include <masl/Callback.h>
#include <masl/XMLNode.h>

namespace spark {

    class Event;
    typedef boost::shared_ptr<Event> EventPtr;
    typedef std::vector<EventPtr> EventPtrList;
    
    template<typename T>
    EventPtr createEvent(const masl::XMLNodePtr theXMLNode) {
        return EventPtr(new T(theXMLNode));
    }

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

            virtual const char * const &  classname_() const = 0;

            std::ostream & print(std::ostream & os) const;

            friend inline std::ostream& operator<<(std::ostream& os, const Event& e) {
                return e.print(os);
            }

            void operator () ();
            void cancelDispatch();
            void startDispatch();
            void finishDispatch();
            void dispatchTo(ComponentPtr theCurrentTarget, EventPhase theCurrentPhase);
            void dispatch();
            const std::string & getType() const {return type_;};
            const ComponentPtr & getTarget() const {return target_;};
            const ComponentPtr & getCurrentTarget() const {return currentTarget_;};
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


    class StageEvent : public Event {
        public:

            StageEvent(const std::string & theType, ComponentPtr theTarget, const masl::UInt64 theCurrentTime);
            StageEvent(const masl::XMLNodePtr theXMLNode);
            virtual ~StageEvent();

            static const char * const CLASSNAME;
            virtual const char * const &  classname_() const {return StageEvent::CLASSNAME;};
            static const char * const FRAME;
            static const char * const PAUSE;
            masl::UInt64 getCurrentTime() const { return currenttime_;};
            masl::UInt64 currenttime_;
    };

    typedef boost::shared_ptr<StageEvent> StageEventPtr;

    class WindowEvent : public Event {
        public:

            WindowEvent(const std::string & theType, ComponentPtr theTarget,
                        const unsigned int theNewWidth, const unsigned int theNewHeight, const unsigned int theOldWidth, const unsigned int theOldHeight);
            WindowEvent(const masl::XMLNodePtr theXMLNode);
            virtual ~WindowEvent();

            static const char * const CLASSNAME;
            virtual const char * const &  classname_() const {return WindowEvent::CLASSNAME;};
            static const char * const ON_RESIZE;
            vector2 size_;
            vector2 oldsize_;
    };

    typedef boost::shared_ptr<WindowEvent> WindowEventPtr;

    class TouchEvent : public Event {
        public:

            TouchEvent(const std::string & theType, ComponentPtr theTarget, const unsigned int theX=0, const unsigned int theY=0);
            TouchEvent(const masl::XMLNodePtr theXMLNode);
            virtual ~TouchEvent();
            static const char * const CLASSNAME;
            virtual const char * const &  classname_() const {return TouchEvent::CLASSNAME;};
            static const char * const TAP;
            static const char * const DOUBLETAP;
            static const char * const LONGPRESS;
            static const char * const PICKED;

            unsigned int getX() const { return x_;};
            unsigned int getY() const { return y_;};

        private:
            unsigned int x_;
            unsigned int y_;
    };

    typedef boost::shared_ptr<TouchEvent> TouchEventPtr;

    class GestureEvent : public Event {
        public:

            GestureEvent(const std::string & theType, ComponentPtr theTarget, const unsigned int theX, const unsigned int theY, const int dx, const int dy);
            GestureEvent(const std::string & theType, ComponentPtr theTarget, const float theFactor);
            GestureEvent(const std::string & theType, ComponentPtr theTarget, const std::string & theDirection);
            GestureEvent(const masl::XMLNodePtr theXMLNode);
            virtual ~GestureEvent();

            static const char * const CLASSNAME;
            virtual const char * const &  classname_() const {return GestureEvent::CLASSNAME;};
            static const char * const PAN;
            static const char * const PINCH;
            static const char * const ROTATE;
            static const char * const SWIPE_LEFT;
            static const char * const SWIPE_RIGHT;

            unsigned int getX() const { return x_;};
            unsigned int getY() const { return y_;};
            int getTranslateX() const { return dx_;};
            int getTranslateY() const { return dy_;};
            float getFactor() const { return factor_;};


        private:
            unsigned int x_;
            unsigned int y_;
            int dx_;
            int dy_;
            float factor_;
            std::string direction_;

    };

    typedef boost::shared_ptr<GestureEvent> GestureEventPtr;

    class SensorEvent : public Event {
        public:

            SensorEvent(const std::string & theType, ComponentPtr theTarget, const float theValue0, const float theValue1, const float theValue2);
            SensorEvent(const masl::XMLNodePtr theXMLNode);
            virtual ~SensorEvent();

            static const char * const CLASSNAME;
            virtual const char * const &  classname_() const {return SensorEvent::CLASSNAME;};

			static const char * const ACCELEROMETER;
			static const char * const GRAVITY;
			static const char * const GYROSCOPE;
			static const char * const LIGHT;
			static const char * const LINEAR_ACCELERATION;
			static const char * const MAGNETIC_FIELD;
			static const char * const ORIENTATION;
			static const char * const PRESSURE;
			static const char * const PROXIMITY;
			static const char * const ROTATION_VECTOR;
			static const char * const TEMPERATURE;

            float getValue0() const { return value0_;};
            float getValue1() const { return value1_;};
            float getValue2() const { return value2_;};

        private:
            float value0_;
			float value1_;
			float value2_;
    };

    typedef boost::shared_ptr<SensorEvent> SensorEventPtr;


    class I18nEvent : public Event {
        public:
            I18nEvent(const masl::XMLNodePtr theXMLNode);
            I18nEvent(const std::string & theType, ComponentPtr theTarget);
            virtual ~I18nEvent();
            static const char * const CLASSNAME;
            static const char * const ON_LANGUAGE_SWITCH;
            virtual const char * const &  classname_() const {return I18nEvent::CLASSNAME;};
    };
    typedef boost::shared_ptr<I18nEvent> I18nEventPtr;



    ////////////////////////////////////////////////////////////////////////
    //Event Callbacks //////////////////////////////////////////////////////

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
