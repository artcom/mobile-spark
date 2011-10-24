// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_masl_file_functions_
#define _included_masl_file_functions_

#include "Exception.h"

#include <string>
#include <vector>

namespace masl {

    DECLARE_EXCEPTION(OpenDirectoryFailed, Exception)
    DECLARE_EXCEPTION(OpenFileFailed, Exception)
    DECLARE_EXCEPTION(FileNotFoundException, Exception)

    /// read a complete file into a string
    bool fileExists(const std::string & theFilename);
    bool readFile(const std::string & thePath, std::string & theContent);
    bool readBinaryFile(const std::string & theUTF8Filename, std::vector<char> & theContent);
    std::string getFilenamePart(const std::string & theFileName);
    std::string getDirectoryPart(const std::string & theFileName);
    void getDirectoryEntries(const std::string & thePath,  std::vector<std::string> & theDirEntries, const std::string & theFilter);
    bool readFileLineByLine(const std::string & theUTF8Filename, std::vector<std::string> & theContent);
    bool searchFile(const std::string & theFileName, std::string & retPath, const bool theForce = false);
    std::string lastFileNamePart(const char* file_name);
    std::string getExtension(const std::string & thePath);
    std::string removeExtension(const std::string & theFileName);
}

#endif

