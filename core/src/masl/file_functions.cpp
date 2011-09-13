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
    bool 
    readFile(const std::string & theUTF8Filename, std::string & theContent) {
         FILE * pFile;
         std::string filepath;
         searchFile(theUTF8Filename, filepath, true);
         char *myCharBuffer;
         pFile = fopen (filepath.c_str(),"r");
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
            free(myCharBuffer);
        }
        return true;
    }

    bool 
    readFileLineByLine(const std::string & theUTF8Filename, std::vector<std::string> & theContent) {
        const size_t MAX_LENGTH = 1000;
        char buffer[MAX_LENGTH];
        std::string newPart;
        std::string filepath;
        searchFile(theUTF8Filename, filepath, true);
        FILE *file;
        if ((file = fopen(filepath.c_str(), "rb")) == NULL) {
            throw Exception("Error opening file");
        }
        size_t size = fread(buffer, 1, MAX_LENGTH,file);
        bool endedWithNewLine = false;
        while (size > 0) {
            newPart = std::string(buffer, size);
            std::stringstream stream(newPart);
            std::string item;
            bool first = true;
            while (std::getline(stream, item, '\n')) {
                if (first && !endedWithNewLine && theContent.size() >0) {
                    theContent.back().append(item);
                } else {
                    theContent.push_back(item);
                }
                first = false;
            }
            endedWithNewLine = (item.size() == 0);
            size = fread(buffer, 1, MAX_LENGTH,file);
        }
        fclose(file);
        return true;
    }

    bool
    searchFile(const std::string & theFileName, std::string & retPath, bool theForce) {
        FILE * pFile;
        pFile = fopen(theFileName.c_str(),"r");
        if (pFile == NULL && theForce) { 
            throw Exception("Error opening file " + theFileName);
        }
        if (pFile) {
            fclose(pFile);
            retPath = theFileName;
            return true;
        }
        return false;
    }

    bool
    searchFile(const std::vector<std::string> & theIncludeList, const std::string & theFileName, std::string & retPath) {
        for (std::vector<std::string>::const_iterator it = theIncludeList.begin(); it != theIncludeList.end(); ++it) {
            if (searchFile((*it) + theFileName, retPath)) {
                return true;
            }
        }
        return false;
    }
}

