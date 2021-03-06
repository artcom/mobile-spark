// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

// own header
#include "string_functions.h"
#include <boost/crc.hpp>

using namespace std;

namespace masl {

    int 
    as_int(const string & theString) {
        int outValue;
        istringstream myStream(theString);
        myStream >> outValue;
        if (!myStream) {
            throw ParseException("as_int","could not convert to integer");
        }
        return outValue;
    }

    float 
    as_float(const string & theString) {
        float outValue;
        istringstream myStream(theString);
        myStream >> outValue;
        if (!myStream) {
            throw ParseException("as_float","could not convert to float");
        }
        return outValue;
    }

    template <>
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

    bool 
    createFloatBinFromString(const std::string & theString, vector<float> & theBin) {
        theBin.clear();
        size_t myStartIndex = 1;
        size_t myEndIndex = theString.find(",", myStartIndex);
        while (myEndIndex != string::npos) {
            std::string s = theString.substr(myStartIndex, myEndIndex-myStartIndex);
            float f = as_float(s);
            theBin.push_back(f);
            myStartIndex = myEndIndex+1;
            myEndIndex = theString.find(",", myStartIndex);
            if (myEndIndex == string::npos) {
                theBin.push_back(as_float(theString.substr(myStartIndex, theString.size()-1-myStartIndex)));
            }
        }
        return true;
    }

    template <>
    bool fromString(const std::string & theString, vector4 & outValue) {
        vector<float> myBin;
        createFloatBinFromString(theString, myBin);
        if (myBin.size() != 4 ) {
            return false;
        }
        for(size_t i = 0; i < myBin.size(); i++) {
            outValue[i] = myBin[i];
        }
        return true;
    }

    template <>
    bool fromString(const std::string & theString, vector3 & outValue) {
        vector<float> myBin;
        createFloatBinFromString(theString, myBin);
        if (myBin.size() != 3 ) {
            return false;
        }
        for(size_t i = 0; i < myBin.size(); i++) {
            outValue[i] = myBin[i];
        }
        return true;
    }

    template <>
    bool fromString(const std::string & theString, vector2 & outValue) {
        vector<float> myBin;
        createFloatBinFromString(theString, myBin);
        if (myBin.size() != 2 ) {
            return false;
        }
        for(size_t i = 0; i < myBin.size(); i++) {
            outValue[i] = myBin[i];
        }
        return true;
    }
    
    unsigned int CRC32 (const std::string & theString) {
        boost::crc_32_type result;
        result.process_bytes(theString.data(), theString.length());
        return result.checksum();        
    }

}

