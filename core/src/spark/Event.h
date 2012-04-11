// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_spark_Event_h_included_
#define _ac_mobile_spark_Event_h_included_

#include <boost/enable_shared_from_this.hpp>
#include <masl/Settings.h>
#include <masl/Callback.h>
#include <masl/XMLNode.h>

namespace spark {
        
    class Event;
    typedef masl::Ptr<Event> EventPtr;
    typedef std::vector<EventPtr> EventPtrList;
    
    template<typename T>
    EventPtr createEvent(const masl::XMLNodePtr theXMLNode) {
        return EventPtr(new T(theXMLNode));
    }

    class Component;
    typedef masl::Ptr<Component> ComponentPtr;

    class Event : public boost::enable_shared_from_this<Event> {
        public:

            Event(const std::string & theType, ComponentPtr theTarget);
            Event(const masl::XMLNodePtr theXMLNode);
            virtual ~Event() = 0;

            enum EventPhase {
                IDLE,
                CAPTURING,
                TARGET,
                BUBBLING
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
            bool isSystemRelevant() const {return systemrelevant_;};
            void connect(ComponentPtr theComponent) {target_ = theComponent;};
        private:
            std::string type_;
            ComponentPtr target_;
            ComponentPtr currentTarget_;
            EventPhase currentPhase_;
            bool dispatching_;
            bool cancelable_;
        protected:
            bool systemrelevant_; // systemrelevant events cannot be discarded or delayed by any event filtering
    };


    class StageEvent : public Event {
        public:

            StageEvent(const std::string & theType, ComponentPtr theTarget, const masl::UInt64 theCurrentTime);
            StageEvent(const masl::XMLNodePtr theXMLNode);
            virtual ~StageEvent();

            static const char * const CLASSNAME;
            virtual const char * const &  classname_() const {return StageEvent::CLASSNAME;};
            static const char * const FRAME;
            masl::UInt64 getCurrentTime() const { return currenttime_;};
            masl::UInt64 getDeltaTime() const { return deltaT_;};
        private:
            masl::UInt64 currenttime_;
            masl::UInt64 deltaT_;
    };

    typedef masl::Ptr<StageEvent> StageEventPtr;

    class WindowEvent : public Event {
        public:

            WindowEvent(const std::string & theType, ComponentPtr theTarget,
                        const unsigned int theNewWidth, const unsigned int theNewHeight, const unsigned int theOldWidth, const unsigned int theOldHeight);
            WindowEvent(const masl::XMLNodePtr theXMLNode);
            WindowEvent(const std::string & theType, ComponentPtr theTarget, const std::string & theWorldname);
                
            virtual ~WindowEvent();

            static const char * const CLASSNAME;
            virtual const char * const &  classname_() const {return WindowEvent::CLASSNAME;};
            static const char * const ON_RESIZE;
            static const char * const WORLD_REALIZED;
            vector2 getSize() const { return size_;};
        private:
            vector2 size_;
            vector2 oldsize_;
            std::string worldname_;
    };

    typedef masl::Ptr<WindowEvent> WindowEventPtr;

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
            static const char * const DOWN;
            static const char * const UP;

            unsigned int getX() const { return x_;};
            unsigned int getY() const { return y_;};

        private:
            unsigned int x_;
            unsigned int y_;
    };

    typedef masl::Ptr<TouchEvent> TouchEventPtr;

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
            static const char * const SWIPE_UP;            
            static const char * const SWIPE_DOWN;

            unsigned int getX() const { return x_;};
            unsigned int getY() const { return y_;};
            int getTranslateX() const { return dx_;};
            int getTranslateY() const { return dy_;};
            float getFactor() const { return factor_;};
            const std::string & getDirection() const { return direction_;};


        private:
            unsigned int x_;
            unsigned int y_;
            int dx_;
            int dy_;
            float factor_;
            std::string direction_;

    };

    typedef masl::Ptr<GestureEvent> GestureEventPtr;

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

    typedef masl::Ptr<SensorEvent> SensorEventPtr;


    class I18nEvent : public Event {
        public:
            I18nEvent(const masl::XMLNodePtr theXMLNode);
            I18nEvent(const std::string & theType, ComponentPtr theTarget);
            virtual ~I18nEvent();
            static const char * const CLASSNAME;
            static const char * const ON_LANGUAGE_SWITCH;
            virtual const char * const &  classname_() const {return I18nEvent::CLASSNAME;};
    };
    typedef masl::Ptr<I18nEvent> I18nEventPtr;

    class ApplicationEvent : public Event {
        public:
            ApplicationEvent(const masl::XMLNodePtr theXMLNode);
            virtual ~ApplicationEvent();
            static const char * const CLASSNAME;
            virtual const char * const &  classname_() const {return ApplicationEvent::CLASSNAME;};
            static const char * const GENERIC;
            static const char * const BUTTON_BACK;
            std::string getAttribute(const std::string & theKey);
        private:
            std::map<std::string, std::string> data_;
    };
    typedef masl::Ptr<ApplicationEvent> ApplicationEventPtr;


    ////////////////////////////////////////////////////////////////////////
    //Event Callbacks //////////////////////////////////////////////////////

    class EventCallback {
    public:
        virtual ~EventCallback() {};
        virtual void execute(EventPtr theEvent) const = 0;
        virtual void operator() (EventPtr theEvent) {execute(theEvent);};
    };

    typedef masl::Ptr<EventCallback> EventCallbackPtr;




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
    typedef masl::Ptr<FreeFunctionEventCallback> FreeFunctionEventCallbackPtr;

    //TP should be weakPtr
    template < typename T, typename TP>
    class MemberFunctionEventCallback : public EventCallback {
    public:
        MemberFunctionEventCallback(TP theObject, void (T::*theFunctionPtr)(EventPtr)):
            _myObjectPtr(theObject),
            _myFunctionPointer(theFunctionPtr) {
        };
        virtual ~MemberFunctionEventCallback() {};

        virtual void execute(EventPtr theEvent) const {
            if (_myObjectPtr.lock()) {
                (_myObjectPtr.lock().get()->*_myFunctionPointer)(theEvent);
            }
        };
    private:
        TP _myObjectPtr;
        void (T::*_myFunctionPointer)(EventPtr);
    };

}
#endif
