//own header
#include "Exception.h"

#include <cstdlib>

#include "string_functions.h"
using namespace std;

namespace masl {


    // TODO: make this a static function
    bool abortOnThrow() {
        static bool myAbortOnThrowFlag = false;//masl::get_environment_var_as("AC_ABORT_ON_THROW", myDummy);
        return myAbortOnThrowFlag;
    }
    Exception::Exception(const std::string & what, const std::string & where)
            : _what(what), _where(where), _name("Exception")
    {
        if (abortOnThrow()) {
            cerr << *this;
            abort();
        }
    };

    Exception::Exception(const std::string & what, const std::string & where, const char * name)
            : _what(what), _where(where), _name(name)
    {
        if (abortOnThrow()) {
            cerr << *this;
            abort();
        }
    };

    void
    Exception::initExceptionBehaviour() {
    }
}



string
masl::compose_message(const Exception& ex) {
    std::string myMsg;
    myMsg =
        string(ex.name() ? ex.name() : "Unspecified Exception") +
        string(": ") +
        string(ex.what().size() ? ex.what() : string("Unspecified reason")) +
        string(" ") +
        string(ex.where().size() ? ex.where() : string("Unspecified origin"));
    return myMsg;
}
