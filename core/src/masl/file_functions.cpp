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
//    $RCSfile: file_functions.cpp,v $
//
//   $Revision: 1.23 $
//
// Description: file helper functions
//
//
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
*/

// own header
#include "file_functions.h"

#include "Exception.h"

#include <string>
#include <stdio.h>

using namespace std;

namespace masl {


    /// read a complete file into a string
    bool readFile(const std::string & theUTF8Filename, std::string & theContent) {
        FILE * pFile;
        char myCharBuffer[65536];
        pFile = fopen (theUTF8Filename.c_str(),"r");
        if (pFile == NULL) { 
            throw Exception("Error opening file");
        } else {
          fgets (myCharBuffer , 65536 , pFile);            
          fclose (pFile);
          theContent = std::string(myCharBuffer);
        }
        return true;
    }
}

