// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "Exception.h"

#include <cstdlib>

#include "string_functions.h"
#include "Logger.h"

using namespace std;

namespace masl {
    DEFINE_EXCEPTION(ParseException, Exception);
    DEFINE_EXCEPTION(NotYetImplemented, Exception)
    DEFINE_EXCEPTION(InputOutputFailure, Exception)

    void
    Exception::appendWhat(const std::string & whatelse) {
        _what += "\n"+whatelse;
    }

    Exception::Exception(const std::string & what, const std::string & where)
            : _what(what), _where(where), _name("Exception")
    {
        _callstack.update();
    };

    Exception::Exception(const std::string & what, const std::string & where, const char * name)
            : _what(what), _where(where), _name(name)
    {
        _callstack.update();
    };

    std::ostream&
    operator<<(std::ostream& os, const Exception & ex) {
        return ex.compose_message(os);
    }

    std::ostream &
    Exception::compose_message(std::ostream& os) const {
        _callstack.dump();
        std::string myMsg;
        myMsg =
            string(name() ? name() : "Unspecified Exception") +
            string(": ") +
            string(what().size() ? what() : string("Unspecified reason")) +
            string(" ") +
            string(where().size() ? where() : string("Unspecified origin"));
        return os << myMsg << std::endl;
    }
}



