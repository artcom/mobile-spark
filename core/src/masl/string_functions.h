#ifndef _included_mobile_masl_string_functions_
#define _included_mobile_masl_string_functions_

#include <sstream>
#include <algorithm>
#include "Exception.h"
#include "numeric_functions.h"
#include "file_functions.h"

namespace masl {

    template <class T>
    inline std::string as_string(const T& x) {
        std::ostringstream streamer;
        streamer << x;
        return streamer.str();
    }

    template <class T>
    inline bool fromString(const std::string & theString, T & outValue) {
        std::istringstream myStream(theString);
        myStream >> outValue;
        return (myStream != 0); // Shouldn't this be (myStream.good() || myStream.eof()) ? (MS)
                                // PM: it would be the same, stream::operator void*() is called here which
                                // return 0 whhen neither the fail nor the bad bit is set, and ignores eofbit
                                // while good() considers all three existing error flagss.
    }


    template <>
    bool fromString(const std::string & theString, bool & outValue);
    template <>
    bool fromString(const std::string & theString, vector4 & outValue);
    template <>
    bool fromString(const std::string & theString, vector3 & outValue);
    template <>
    bool fromString(const std::string & theString, vector2 & outValue);

    template <>
    inline bool fromString(const std::string & theString, std::string & outValue) {
        outValue = theString;
        return true;
    };

    template <class T>
    T
    as(const std::string & theString) {
        T outValue;
        if (!masl::fromString(theString,outValue)) {
            throw ParseException("as<T>",std::string("could not convert '")+theString);
        };
        return outValue;
    }

    int as_int(const std::string & theString);
    float as_float(const std::string & theString);

    inline std::string
    line_string(unsigned line_number) {
        std::string message;
        message = as_string(line_number);
        return message;
    }

    inline std::string
    file_string(const char* file_name) {
        std::string myFilename(file_name);
        std::string::size_type mySlash = myFilename.find_last_of("/\\");
        if (mySlash != std::string::npos) {
            myFilename = myFilename.substr(mySlash+1);
        }
        return myFilename;
    }

    inline std::string
    location_string(const char* file_name, unsigned line_number) {
        return std::string("[") + lastFileNamePart(file_name) + ":" + as_string(line_number) + "]";
    }


    #define PLUS_FILE_LINE masl::location_string(__FILE__,__LINE__)
    #define JUST_FILE_LINE masl::line_string(__LINE__),masl::file_string(__FILE__)


    inline std::string trimLeft (const std::string & theString, const std::string & theTrimChars = " " ) {
        std::string myString(theString);
        return myString.erase(0, theString.find_first_not_of ( theTrimChars ));
    }

    inline std::string trimRight (const std::string & theString, const std::string & theTrimChars = " ") {
        std::string myString(theString);
        return myString.erase(theString.find_last_not_of ( theTrimChars ) + 1);
    }

    inline std::string trim (const std::string & theString, const std::string & theTrimChars = " ") {
        std::string myString(theString);
        return trimLeft ( trimRight ( myString, theTrimChars), theTrimChars);
    }
    inline std::string trimall (const std::string & theString) {
        std::string myString(theString);
        myString = masl::trim(myString, " ");
        myString = masl::trim(myString, "\t");
        myString = masl::trim(myString, "\n");
        return masl::trim(myString, "\r");
    }

    template <typename Pair>
    struct select1st : std::unary_function<Pair, typename Pair::first_type> 
    {
        const typename Pair::first_type& operator()(const Pair& x) const {
            return x.first;
        }
    };
    
} //Namespace masl

#endif

