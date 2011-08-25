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
#include "Logger.h"

using namespace std;

namespace masl {


    /// read a complete file into a string
    bool readFile(const std::string & theUTF8Filename, std::string & theContent) {
        FILE * pFile;
        char *myCharBuffer;
        pFile = fopen (theUTF8Filename.c_str(),"r");
        if (pFile == NULL) { 
            throw Exception("Error opening file");
        } else {
            fseek(pFile,0,SEEK_END); //go to end
            int len=ftell(pFile); //get position at end (length)
            fseek(pFile,0,SEEK_SET); //go to beg.
            myCharBuffer=(char *)malloc(len + 1); //malloc buffer
            memset(myCharBuffer, 0, len + 1);
            fread(myCharBuffer,len,1,pFile); //read into buffer
            fclose(pFile);
            theContent = std::string(myCharBuffer);
            free (myCharBuffer);
        }
        return true;

    }
}

