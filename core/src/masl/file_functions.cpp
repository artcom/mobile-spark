// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

// own header
#include "file_functions.h"
#include "Exception.h"
#include "AssetProvider.h"


#include <string>
#include <stdio.h>
#include "Logger.h"

#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <dirent.h>

#ifdef ANDROID
#define STAT64 stat64
#endif
#ifdef iOS
#define STAT64 stat
#endif

#define BOOST_FILESYSTEM_VERSION 2
#include <boost/filesystem.hpp>


using namespace std;

namespace masl {

    DEFINE_EXCEPTION(OpenDirectoryFailed, Exception)
    DEFINE_EXCEPTION(OpenFileFailed, Exception)
    DEFINE_EXCEPTION(FileNotFoundException, Exception)

    // boosted
    int 
    getFileSize(const std::string & theFilename) {
        return boost::filesystem::file_size(theFilename.c_str());
    }

    /// read a complete file into a string
    bool
    readFile(const std::string & theUTF8Filename, std::string & theContent) {
         FILE * pFile;
         std::string filepath;            
         searchFile(theUTF8Filename, filepath, true);
         
         char *myCharBuffer;
         pFile = fopen (filepath.c_str(),"r");
         if (pFile == NULL) {
             throw OpenFileFailed("Error opening file " + theUTF8Filename, PLUS_FILE_LINE);
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
    readBinaryFile(const std::string & theUTF8Filename, std::vector<char> & theContent) {
         FILE * pFile;
         std::string filepath;
         searchFile(theUTF8Filename, filepath, true);
         char *myCharBuffer;
         pFile = fopen (filepath.c_str(),"rb");
         if (pFile == NULL) {
             throw OpenFileFailed("Error opening file " + theUTF8Filename, PLUS_FILE_LINE);
         } else {
            fseek(pFile,0,SEEK_END); //go to end
            int len=ftell(pFile); //get position at end (length)
            fseek(pFile,0,SEEK_SET); //go to beg.
            myCharBuffer=(char *)malloc(len + 1); //malloc buffer
            memset(myCharBuffer, 0, len + 1);
            fread(myCharBuffer,len,1,pFile); //read into buffer
            fclose(pFile);
            copy(myCharBuffer, myCharBuffer + len, back_inserter(theContent));
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
            throw OpenFileFailed("Error opening file " + theUTF8Filename, PLUS_FILE_LINE);
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

    std::string 
    getFilenamePart(const std::string & theFileName) {
        std::string myBaseName;
        if ( ! theFileName.empty()) {
            if (theFileName.at(theFileName.length()-1) == '/') {  // Huh? what's that for??? [DS/MS]
                // return empty string if theFileName ends with "/"
                return std::string("");
            }

            char * myBuffer = strdup(theFileName.c_str());
            myBaseName = basename(myBuffer);
            free(myBuffer);
        }
        return myBaseName;
    }

    bool 
    fileExists(const std::string& theFilename) {
        struct STAT64 myStat;
        return stat(theFilename.c_str(), &myStat) != -1;
    }

    void 
    getDirectoryEntries(const std::string & thePath,  std::vector<std::string> & theDirEntries, const std::string & theFilter) {
        if (!fileExists(thePath)) {
            return;
        }
        DIR * myDirHandle = opendir(thePath.c_str());
        if (!myDirHandle) {
            throw OpenDirectoryFailed(string("thePath='") + thePath + "'not found", PLUS_FILE_LINE);
        }
        struct dirent *dir_entry;
        while((dir_entry = readdir(myDirHandle)) != 0) {
            if (std::string("..")!= dir_entry->d_name && std::string(".") != dir_entry->d_name) {
                if (theFilter == "" || string(dir_entry->d_name).find(theFilter) != string::npos) {
                    theDirEntries.push_back(dir_entry->d_name);
                }
            }
        }
        closedir(myDirHandle);
    }

    bool
    searchFile(const std::string & theFileName, std::string & retPath, bool theForce) {
        const std::vector<std::string> & myIncludeList = masl::AssetProviderSingleton::get().ap()->getIncludePaths();
        for (std::vector<std::string>::const_iterator it = myIncludeList.begin(); it != myIncludeList.end(); ++it) {
            if (fileExists((*it) + theFileName)) {
                retPath = (*it) + theFileName;
                return true;
            }
        }
        if (theForce) {
            throw OpenFileFailed("Error opening file " + theFileName, PLUS_FILE_LINE);            
        }
        return false;
    }

    std::string 
    getDirectoryPart(const std::string & theFileName) {
        std::string myDirName;
        if (! theFileName.empty() ) {
            if (theFileName.at(theFileName.length()-1) == '/') {
                return theFileName;
            }

            char * myBuffer = strdup(theFileName.c_str());
            myDirName = dirname(myBuffer);
            free(myBuffer);
            if (!myDirName.empty() &&
                myDirName.at(myDirName.length()-1) != '/')
            {
                myDirName += "/";
            }
        }

        return myDirName;
    }

    std::string
    lastFileNamePart(const char* file_name) {
        std::string myFilename(file_name);
        std::string::size_type mySlash = myFilename.find_last_of("/\\");
        if (mySlash != std::string::npos) {
            myFilename = myFilename.substr(mySlash+1);
        }
        return myFilename;
    }

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

}

