// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "AndroidAssetProvider.h"

#include <iostream>
#include <fstream>

#include <masl/Logger.h>
#include <masl/file_functions.h>
#include <masl/MobileSDK.h>

#include "APK_functions.h"

using namespace std;
namespace android {

    AndroidAssetProvider::AndroidAssetProvider(const std::string & theApkPath, const std::string & theAppPath)
        : AssetProvider(), _myApkArchive(NULL)
    {
        android::loadAPK(&_myApkArchive, theApkPath);
        assetPath_ =  "/sdcard/" + theAppPath;
        includePaths_.push_back("");
    }

    AndroidAssetProvider::~AndroidAssetProvider() {
        if (_myApkArchive) {
            zip_close(_myApkArchive);
        }
    }

    void
    AndroidAssetProvider::addIncludePath(const std::string & thePath) {
        includePaths_.push_back(assetPath_ + "/" + thePath);
        includePaths_.push_back("assets/" + thePath);
    }

    std::string
    AndroidAssetProvider::getStringFromFile(const std::string & theFileName) const {
        if (theFileName.size() > 0 ) {
            //unzipped, read from sdcard
            std::string myContent;
            std::string filePath;
            if (masl::searchFile(theFileName, filePath)) {
                masl::readFile(filePath, myContent);
                return myContent;
            }
        }
        return readFromPackage(_myApkArchive, theFileName);
    }

    std::vector<char>
    AndroidAssetProvider::getBlockFromFile(const std::string & theFileName) const {
        if (theFileName.size() > 0) {
            //unzipped, read from sdcard
            std::vector<char> myContent;
            std::string filePath;
            if (masl::searchFile(theFileName, filePath)) {
                masl::readBinaryFile(filePath, myContent);
                return myContent;
            }
        }
        return readBinaryFromPackage(_myApkArchive, theFileName);
    }

    std::vector<std::string>
    AndroidAssetProvider::getLineByLineFromFile(const std::string & theFileName) const {
        if (theFileName.size() > 0) {
            //unzipped, read from sdcard
            std::vector<std::string> myContent;
            std::string filePath;
            if (masl::searchFile(theFileName, filePath)) {
                masl::readFileLineByLine(filePath, myContent);
                return myContent;
            }
        }
        return readLineByLineFromPackage( _myApkArchive, theFileName);
    }

    bool
    AndroidAssetProvider::loadTextureFromFile(const std::string & filename, unsigned int & textureId,
                                              unsigned int & width, unsigned int & height,
                                              unsigned int & realwidth, unsigned int & realheight,
                                              matrix & npotMatrix,
                                              bool & hasAlpha, bool & theMirrorFlag, bool & theMipMapFlag) {
        theMirrorFlag = true;
        std::string myFullPath = filename;
        bool myFileIsonSDCardFlag = masl::searchFile(filename, myFullPath);
        if (!myFileIsonSDCardFlag) {
            searchFile(_myApkArchive, filename, myFullPath, true);
        }
        return masl::MobileSDK_Singleton::get().getNative()->loadTextureFromFile(myFullPath,
                                                                                 textureId,
                                                                                 width,
                                                                                 height,
                                                                                 realwidth,
                                                                                 realheight,
                                                                                 npotMatrix,
                                                                                 hasAlpha,
                                                                                 theMipMapFlag);
    }


    void
    AndroidAssetProvider::storeInFile(const std::string & theFileName, const std::string & theData) {
        std::ofstream myfile(std::string(assetPath_ + "/" + theFileName).c_str());
        myfile << theData;
        myfile.close();
    }

    void
    AndroidAssetProvider::storeInFile(const std::string & theFileName, const std::vector<char> & theData) {
        std::ofstream myfile(std::string(assetPath_ + "/" + theFileName).c_str(),ofstream::binary);
        myfile.write(&theData[0],theData.size());
        myfile.close();
    }

    std::string AndroidAssetProvider::getDownloadsPath() const {
        return getAssetPath() + getDownloadsFolder();
    }

    std::string AndroidAssetProvider::getDownloadsFolder() const {
        return "/downloads/";
    }

    //if theForceOnlyInBundle_APK: get only files from apk
    //if !theForceOnlyInBundle_APK: get files form sdcard. If there are none, get files from apk.
    std::vector<std::string>
    AndroidAssetProvider::getFilesFromPath(const std::string & thePath, const std::string & thePattern, const bool theForceOnlyInBundle_APK) const {
        std::vector<std::string> myFiles;
        if (!theForceOnlyInBundle_APK) {
            myFiles = AssetProvider::getFilesFromPath(thePath, thePattern);
        }
        if (myFiles.size() == 0) {
            android::getDirectoryEntries(_myApkArchive, thePath, myFiles, thePattern);
        }
        return myFiles;
    }

}

