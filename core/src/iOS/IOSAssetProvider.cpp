// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "IOSAssetProvider.h"

#include <iostream>
#include <fstream>

#include <masl/file_functions.h>
#include <mar/png_functions.h>



namespace ios
{
    IOSAssetProvider::IOSAssetProvider(const std::string & theAssetFolderPath, const std::string & theAppPath)
    : AssetProvider(), _myAssetFolderPath(theAssetFolderPath)
    {
        assetPath_ = theAssetFolderPath + "/" + theAppPath;
    }

    IOSAssetProvider::~IOSAssetProvider()
    {
    }

    bool IOSAssetProvider::loadTextureFromPNG(const std::string & theFile, unsigned int & textureId, int & outWidth, int & outHeight, bool & rgb)
    {
        if (theFile.size() > 0 && theFile[0] == '/') {
            std::string filePath;
            if (masl::searchFile(includePaths_, theFile, filePath)) {
                return mar::loadTextureFromPNG(filePath, textureId, outWidth, outHeight, rgb);
            }
            AC_ERROR << "texture " << theFile << " was not found in search paths";
            throw masl::FileNotFoundException("texture " + theFile + " was not found in search paths", PLUS_FILE_LINE);
        }
        return mar::loadTextureFromPNG(_myAssetFolderPath + "/" + theFile, textureId, outWidth, outHeight, rgb);
    }

    std::string
    IOSAssetProvider::getStringFromFile(const std::string & theFile) const {
        std::string content = "";
        if (theFile.size() > 0 && theFile[0] == '/') {
            std::string filePath;
            if (masl::searchFile(includePaths_, theFile, filePath)) {
                masl::readFile(filePath, content);
            } else {
                AC_ERROR << "file " << theFile << " was not found in search paths";
                throw masl::FileNotFoundException("file " + theFile + " was not found in search paths", PLUS_FILE_LINE);
            }
            return content;
        }
        masl::readFile(_myAssetFolderPath + "/" + theFile, content);
        return content;
    }

    std::vector<char>
    IOSAssetProvider::getBlockFromFile(const std::string & theFile) const {
        std::vector<char> content;
        if (theFile.size() > 0 && theFile[0] == '/') {
            std::string filePath;
            if (masl::searchFile(includePaths_, theFile, filePath)) {
                masl::readBinaryFile(filePath, content);
            } else {
                AC_ERROR << "file " << theFile << " was not found in search paths";
                throw masl::FileNotFoundException("file " + theFile + " was not found in search paths", PLUS_FILE_LINE);
            }
            return content;
        }
        masl::readBinaryFile(_myAssetFolderPath + "/" + theFile, content);
        return content;
    }

    std::vector<std::string>
    IOSAssetProvider::getLineByLineFromFile(const std::string & theFile) const {
        std::vector<std::string> content;
        if (theFile.size() > 0 && theFile[0] == '/') {
            std::string filePath;
            if (masl::searchFile(includePaths_, theFile, filePath)) {
                masl::readFileLineByLine(filePath, content);
            } else {
                AC_ERROR << "file " << theFile << " was not found in search paths";
                throw masl::FileNotFoundException("file " + theFile + " was not found in search paths", PLUS_FILE_LINE);
            }
            return content;
        }
        masl::readFileLineByLine(_myAssetFolderPath + "/" + theFile, content);
        return content;
    }

    void 
    IOSAssetProvider::storeInFile(const std::string & theFileName, const std::string & theData) {
        std::ofstream myfile(std::string(assetPath_ + "/../../Documents/" + theFileName).c_str());
        myfile << theData;
        myfile.close();
    }

    void 
    IOSAssetProvider::storeInFile(const std::string & theFileName, const std::vector<char> & theData) {
        std::ofstream myfile(std::string(assetPath_ + "/../../Documents/" + theFileName).c_str(),std::ofstream::binary);
        myfile.write(&theData[0],theData.size());
        myfile.close();
    }
    
    std::string IOSAssetProvider::getDownloadsPath() const {
        return getAssetPath()+getDownloadsFolder();
    }
    
    std::string IOSAssetProvider::getDownloadsFolder() const {
        return "/../../Documents/downloads/";
    }
}
