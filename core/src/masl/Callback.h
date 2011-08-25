#ifndef _ac_mobile_masl_Callback_h_included_
#define _ac_mobile_masl_Callback_h_included_

#include <boost/smart_ptr/shared_ptr.hpp>
#include <masl/Logger.h>

namespace masl {
    class Callback {
    public:
        virtual ~Callback() {};
        virtual void execute() const = 0;
    };

    typedef boost::shared_ptr<Callback> CallbackPtr;


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
    typedef boost::shared_ptr<FreeFunctionCallback> FreeFunctionCallbackPtr;

    template < typename BASE, typename DERIVED = BASE>
    class MemberFunctionCallback : public Callback {
    public:
        MemberFunctionCallback(DERIVED theObject, void (BASE::*theFunctionPtr)()): 
            _myObjectPtr(theObject),
            _myFunctionPointer(theFunctionPtr) {
        };
        virtual ~MemberFunctionCallback() {};

        virtual void execute() const {
            (_myObjectPtr.get()->*_myFunctionPointer)();
        };
    private:
        DERIVED _myObjectPtr;
        void (BASE::*_myFunctionPointer)();
    };
};

#endif

