/* __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2008, ART+COM AG Berlin, Germany <www.artcom.de>
//
// This file is part of the ART+COM Standard Library (asl).
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
//    $RCSfile: string_functions.h,v $
//
//   $Revision: 1.16 $
//
// Description: string helper functions
//
//
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
*/
#ifndef _included_mobile_asl_string_functions_
#define _included_mobile_asl_string_functions_

#include <sstream>
#include "Exception.h"

namespace asl {

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


    bool fromString(const std::string & theString, bool & outValue);

    inline
    bool fromString(const std::string & theString, std::string & outValue) {
        outValue = theString;
        return true;
    };

    template <class T>
    T
    as(const std::string & theString) {
        T outValue;
        if (!asl::fromString(theString,outValue)) {
            throw ParseException("as<T>",std::string("could not convert '")+theString);
        };
        return outValue;
    }

    int as_int(const std::string & theString);


} //Namespace asl

#endif

