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
//    $RCSfile: string_functions.cpp,v $
//
//   $Revision: 1.9 $
//
// Description: string helper functions
//
//
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
*/

// own header
#include "string_functions.h"

using namespace std;

namespace asl {

    int as_int(const string & theString) {
        int outValue;
        istringstream myStream(theString);
        myStream >> outValue;
        if (!myStream) {
            throw ParseException("as_int","could not convert to integer");
        }
        return outValue;
    }

    float as_float(const string & theString) {
        float outValue;
        istringstream myStream(theString);
        myStream >> outValue;
        if (!myStream) {
            throw ParseException("as_float","could not convert to float");
        }
        return outValue; 
    }

    bool fromString(const string & theString, bool & outValue) {
        if (theString == "true" || theString=="1") {
            outValue = true;
            return true;
        }
        if (theString == "false" || theString=="0") {
            outValue = false;
            return true;
        }
        return false;
    }
}

