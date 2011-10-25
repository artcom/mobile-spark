// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

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

    //TP should be WeakPtr
    template < typename T, typename TP>
    class MemberFunctionCallback : public Callback {
    public:
        MemberFunctionCallback(TP theObject, void (T::*theFunctionPtr)()):
            _myObjectPtr(theObject),
            _myFunctionPointer(theFunctionPtr) {
        };
        virtual ~MemberFunctionCallback() {};

        virtual void execute() const {
            if (_myObjectPtr.lock()) {
                (_myObjectPtr.lock().get()->*_myFunctionPointer)();
            }
        };
    private:
        TP _myObjectPtr;
        void (T::*_myFunctionPointer)();
    };

};

#endif

