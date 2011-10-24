// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "APK_functions.h"

#include <masl/Logger.h>
#include <masl/AssetProvider.h>

#define DB(x) //x

using namespace std;
namespace android {

    DEFINE_EXCEPTION(APKLoadingException, masl::Exception)

    void loadAPK (zip** theAPKArchive, const string & apkPath) {
        AC_INFO << "Loading APK " << apkPath;
        *theAPKArchive = zip_open(apkPath.c_str(), 0, NULL);
        if (!(*theAPKArchive)) {
            AC_ERROR << "Error loading APK " << apkPath;
            throw APKLoadingException("Error loading APK " + apkPath, PLUS_FILE_LINE);
            return;
        }

        ////Just for debug, print APK contents
        DB(int numFiles = zip_get_num_files(*theAPKArchive);
            for (int i=0; i<numFiles; i++) {
                const char* name = zip_get_name(*theAPKArchive, i, 0);
                if (name == NULL) {
                    AC_ERROR << "Error reading zip file name at index " << zip_strerror(*theAPKArchive);
                    return;
                }
                AC_PRINT << "File " << i >> ": " << name;
            }
        )
    }

    std::string readFromPackage(zip* theAPKArchive, const string &  theFileName) {
        std::string content = "";
        const size_t MAX_LENGTH = 50000;
        std::string myFoundFilePath;
        bool foundFile = searchFile(theAPKArchive, theFileName, myFoundFilePath, true);
        if (!foundFile) {
            return NULL;
        }
        zip_file* file = zip_fopen(theAPKArchive, myFoundFilePath.c_str(), 0);
        
        char buffer[MAX_LENGTH];
        size_t size = zip_fread(file, buffer, MAX_LENGTH);
        content = std::string(buffer, size);
        zip_fclose(file);
        return content;
    }

    std::vector<char> readBinaryFromPackage(zip* theAPKArchive, const string &  theFileName) {
        std::vector<char> content;
        const size_t MAX_LENGTH = 5000;
        std::string myFoundFilePath;
        bool foundFile = searchFile(theAPKArchive, theFileName, myFoundFilePath, true);
        if (!foundFile) {
            return content;
        }
        zip_file* file = zip_fopen(theAPKArchive, myFoundFilePath.c_str(), 0);
        
        char buffer[MAX_LENGTH];
        size_t size = zip_fread(file, buffer, MAX_LENGTH);
        copy(buffer, buffer + size, back_inserter(content));
        zip_fclose(file);
        return content;
    }

    std::vector<std::string> readLineByLineFromPackage(zip* theAPKArchive, const string & theFileName) {
        std::vector<std::string> content;
        const size_t MAX_LENGTH = 1000;
        char buffer[MAX_LENGTH];
        std::string newPart;
        std::string myFoundFilePath;
        bool foundFile = searchFile( theAPKArchive, theFileName, myFoundFilePath, true);
        if (!foundFile) {
            return content;
        }
        zip_file* file = zip_fopen(theAPKArchive, myFoundFilePath.c_str(), 0);
        size_t size = zip_fread(file, buffer, MAX_LENGTH);
        bool endedWithNewLine = false;
        while (size > 0) {
            newPart = std::string(buffer, size);
            std::stringstream stream(newPart);
            std::string item;
            bool first = true;
            while (std::getline(stream, item, '\n')) {
                if (first && !endedWithNewLine && content.size() >0) {
                    content.back().append(item);
                } else {
                    content.push_back(item);
                }
                first = false;
            }
            endedWithNewLine = (item.size() == 0);
            size = zip_fread(file, buffer, MAX_LENGTH);
        }
        zip_fclose(file);
        return content;
    }
 
    bool searchFile(zip* theAPKArchive, const string & theFileName, std::string & retPath, const bool theForce) {
        const std::vector<std::string> & myIncludeList = masl::AssetProviderSingleton::get().ap()->getIncludePaths();
        for (std::vector<std::string>::const_iterator it = myIncludeList.begin(); it != myIncludeList.end(); ++it) {
            if (fileExist(theAPKArchive, (*it) + theFileName)) {   
                retPath = (*it) + theFileName;
                return true;
            }
        }
        if (theForce) {
            AC_ERROR << "Error opening " << theFileName << " from APK";
            throw APKLoadingException("Error opening APK " + theFileName, PLUS_FILE_LINE);
        }        
        return false;
    }
    
    //if file exists, returns reference to zip_file which has to be zip_fclosed by caller
    bool fileExist(zip* theAPKArchive, const string & theFileName) {
        if (!theAPKArchive) {
            AC_ERROR << "apk broken";
            throw APKLoadingException("apk broken " + theFileName, PLUS_FILE_LINE);
        }
        zip_file* file = zip_fopen(theAPKArchive, theFileName.c_str(), 0);
        if (!file) {
            return false;
        }
        zip_fclose(file);
        return true;
    }

    void getDirectoryEntries(zip* theAPKArchive, const std::string & thePath, std::vector<std::string> & theDirEntries, const std::string & theFilter) {
        if (!theAPKArchive) {
            AC_ERROR << "apk broken";
            throw APKLoadingException("apk broken ", PLUS_FILE_LINE);
        }
        int numfilesInZip = zip_get_num_files(theAPKArchive);
        for (int i = 0; i < numfilesInZip; ++i) {
            std::string s = zip_get_name(theAPKArchive, i, ZIP_FL_UNCHANGED);
            AC_DEBUG << s;
            if (s.find("assets/"+thePath) == 0) {
                theDirEntries.push_back(s);
            }
        }
    }

    
}

