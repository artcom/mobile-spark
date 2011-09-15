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
//
// Description:  Cross Platform File I/O helper function
//
// Last Review: pavel 30.11.2005
//
//  review status report: (perfect, ok, fair, poor, disaster)
//    usefullness            : perfect
//    formatting             : ok
//    documentation          : poor
//    test coverage          : ok
//    names                  : ok
//    style guide conformance: ok
//    technical soundness    : ok
//    dead code              : ok
//    readability            : ok
//    understandabilty       : ok
//    interfaces             : ok
//    confidence             : ok
//    integration            : ok
//    dependencies           : ok
//    cheesyness             : ok
//
//    overall review status  : fair
//
//    recommendations:
//       - make a documentation
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
*/
#ifndef _included_masl_file_functions_
#define _included_masl_file_functions_

#include "Exception.h"

#include <string>
#include <vector>

namespace masl {
    
    DEFINE_EXCEPTION(OpenDirectoryFailed, Exception);        
    /// read a complete file into a string
    bool fileExists(const std::string& theFilename);    
    bool readFile(const std::string & thePath, std::string & theContent);
    std::string getFilenamePart(const std::string & theFileName);
    std::string getDirectoryPart(const std::string & theFileName);        
    void getDirectoryEntries(const std::string & thePath,  std::vector<std::string> & theDirEntries, std::string theFilter);       
    bool readFileLineByLine(const std::string & theUTF8Filename, std::vector<std::string> & theContent);
    bool searchFile(const std::string & theFileName, std::string & retPath, const bool theForce = false);
    bool searchFile(const std::vector<std::string> & theIncludeList, const std::string & theFileName, std::string & retPath);
}

#endif

