#ifndef _ac_mobile_masl_Callback_h_included_
#define _ac_mobile_masl_Callback_h_included_

#include <masl/Logger.h>
#include <masl/Ptr.h>

namespace masl {
    class Callback {
    public:
        virtual ~Callback() {};
        virtual void execute() const = 0;
        virtual void operator() () {execute();};
    };

    typedef masl::Ptr<Callback> CallbackPtr;


    typedef void (*FreeFunctionPtr)();

    class FreeFunctionCallback : public Callback {
    public:
        FreeFunctionCallback(FreeFunctionPtr theFunctionPtr):
            Callback(),
            _myFunctionPointer(theFunctionPtr) {
        };
        virtual ~FreeFunctionCallback() {};

        virtual void execute() const {
            _myFunctionPointer();
        };
    private:
        FreeFunctionPtr _myFunctionPointer;
    };
    typedef masl::Ptr<FreeFunctionCallback> FreeFunctionCallbackPtr;

    template < typename T, typename TP>
    class MemberFunctionCallback : public Callback {
    public:
        MemberFunctionCallback(TP theObject, void (T::*theFunctionPtr)()):
            _myObjectPtr(theObject),
            _myFunctionPointer(theFunctionPtr) {
        };
        virtual ~MemberFunctionCallback() {};

        virtual void execute() const {
            (_myObjectPtr.get()->*_myFunctionPointer)();
        };
    private:
        TP _myObjectPtr;
        void (T::*_myFunctionPointer)();
    };

};

#endif

