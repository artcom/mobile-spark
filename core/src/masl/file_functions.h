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
    bool fileExists(const std::string& theFilename);
    bool readFile(const std::string & thePath, std::string & theContent);
    std::string getFilenamePart(const std::string & theFileName);
    std::string getDirectoryPart(const std::string & theFileName);
    void getDirectoryEntries(const std::string & thePath,  std::vector<std::string> & theDirEntries, std::string theFilter);
    bool readFileLineByLine(const std::string & theUTF8Filename, std::vector<std::string> & theContent);
    bool searchFile(const std::string & theFileName, std::string & retPath, const bool theForce = false);
    bool searchFile(const std::vector<std::string> & theIncludeList, const std::string & theFileName, std::string & retPath);
    std::string lastFileNamePart(const char* file_name);
    std::string getExtension(const std::string & thePath);
    std::string removeExtension(const std::string & theFileName);
}

#endif

