// own header
#include "string_functions.h"
#include "Logger.h"

using namespace std;

namespace masl {

    std::string
    getExtension(const std::string & thePath) {
        std::string::size_type myDotPos = thePath.rfind(".");
        if (myDotPos != std::string::npos) {

            std::string::size_type mySlashPos = thePath.rfind("/");
            if (mySlashPos == std::string::npos) {
                mySlashPos = thePath.rfind("\\");
            }
            if (mySlashPos != std::string::npos && mySlashPos > myDotPos) {
                return "";
            }

            return thePath.substr(myDotPos+1);
        }
        return "";
    }

    std::string
    removeExtension(const std::string & theFileName) {
        std::string::size_type myDotPos = theFileName.rfind(".");
        if (myDotPos != std::string::npos) {
            std::string::size_type mySlashPos = theFileName.rfind("/");
            if (mySlashPos != std::string::npos && mySlashPos > myDotPos) {
                return theFileName;
            }
            return theFileName.substr(0, myDotPos);
        }
        return theFileName;
    }

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

    bool createFloatBinFromString(const std::string & theString, vector<float> & theBin) {
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

}

