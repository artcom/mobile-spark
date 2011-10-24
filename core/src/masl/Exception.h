// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_masl_ExceptionBase_
#define _included_mobile_masl_ExceptionBase_

#include <iostream>
#include <string>
#include <stdexcept>

#include "CallStack.h"

namespace masl {

    class Exception {
    public:

        Exception(const std::string & what = "", const std::string & where ="");

        virtual ~Exception() {}

        Exception(const Exception& ex)
            : _what(ex.what()), _where(ex.where()), _name(ex.name()) {};

        Exception& operator=(const Exception& ex) {
            set(ex.what(),ex.where(),ex.name());
            return *this;
        }
        virtual const std::string & what() const { return _what; }
        virtual const std::string & where() const {
            return _where;
        }
        virtual const char * name() const {
            return _name;
        }
        // we need to actually define one non-pure-virtual method out-of-line to generate the vtable
        virtual void appendWhat(const std::string & whatelse);

        std::ostream & compose_message(std::ostream & os) const;

        friend std::ostream& operator<<(std::ostream& os,const Exception & ex);

    protected:
        virtual void set(const std::string & what,
                         const std::string & where,
                         const char * name) const {
            _what = what;
            _where = where;
            _name = name;
        }
        Exception(const std::string & what, const std::string & where, const char * name);

    private:
        mutable std::string _what;
        mutable std::string _where;
        mutable const char * _name;
        CallStack _callstack;

    };



    #define DEFINE_NAMED_EXCEPTION(CLASS,NAME,BASECLASS)\
    class CLASS : public BASECLASS {\
        public:\
               CLASS(const std::string & what, const std::string & where)\
                   : BASECLASS(what,where, # NAME ) {}\
               virtual void appendWhat(const std::string & whatelse);\
        protected:\
                  CLASS(const std::string & what, const std::string & where, const char * name)\
                      : BASECLASS(what,where, name) {}\
    };
#if 1
    #define DECLARE_EXCEPTION(CLASS,BASECLASS) DEFINE_NAMED_EXCEPTION(CLASS,CLASS,BASECLASS)
    // we need to actually define one non-pure-virtual method out-of-line to generate the vtable
    #define DEFINE_EXCEPTION(CLASS,BASECLASS)\
        void\
        CLASS::appendWhat(const std::string & whatelse) { BASECLASS::appendWhat(whatelse); }
#else
    #define DECLARE_EXCEPTION(CLASS,BASECLASS) typedef BASECLASS CLASS;
    #define DEFINE_EXCEPTION(CLASS,BASECLASS)
#endif
    #define DEFINE_NESTED_EXCEPTION(INSIDECLASS,CLASS,BASECLASS) DEFINE_NAMED_EXCEPTION(CLASS,INSIDECLASS::CLASS,BASECLASS)

    // Some generic exceptions
    DECLARE_EXCEPTION(ParseException, Exception);
    DECLARE_EXCEPTION(NotYetImplemented, Exception)
    DECLARE_EXCEPTION(InputOutputFailure, Exception)


} //Namespace masl



#endif
