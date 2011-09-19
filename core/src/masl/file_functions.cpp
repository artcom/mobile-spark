// own header
#include "file_functions.h"

#include "Exception.h"

#include <string>
#include <stdio.h>
#include "Logger.h"

#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <dirent.h>

#ifdef __ANDROID__
#define STAT64 stat64
#endif
#if __APPLE__
#define STAT64 stat
#endif



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

    std::string getFilenamePart(const std::string & theFileName) {
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
    bool fileExists(const std::string& theFilename) {
        struct STAT64 myStat;
        return stat(theFilename.c_str(), &myStat) != -1;
    }

   void getDirectoryEntries(const string & thePath,  std::vector<string> & theDirEntries, string theFilter) {
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
    std::string getDirectoryPart(const std::string & theFileName) {        
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
    
}

