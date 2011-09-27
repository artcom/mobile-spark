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
}



string
masl::compose_message(const Exception& ex) {
    std::string myMsg;
    myMsg =
        string(ex.name() ? ex.name() : "Unspecified Exception") +
        string(": ") +
        string(ex.what().size() ? ex.what() : string("Unspecified reason")) +
        string(" ") +
        string(ex.where().size() ? ex.where() : string("Unspecified origin")) +
        string("\n");

    myMsg += ex.stack().toString();
    return myMsg;
}
