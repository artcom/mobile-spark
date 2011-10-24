#include "AndroidAssetProvider.h"

#include <iostream>
#include <fstream>

#include <masl/Logger.h>
#include <masl/file_functions.h>
#include <mar/png_functions.h>

#include "APK_functions.h"
#include "png_functions.h"

using namespace std;
namespace android {

    AndroidAssetProvider::AndroidAssetProvider(const std::string & theApkPath, const std::string & theAppPath)
        : AssetProvider(), _myApkArchive(NULL)
    {
        android::loadAPK(&_myApkArchive, theApkPath);
        assetPath_ =  "/sdcard/" + theAppPath;
    }

    AndroidAssetProvider::~AndroidAssetProvider() {
        if (_myApkArchive) {
            zip_close(_myApkArchive);
        }
    }
    
    void 
    AndroidAssetProvider::addIncludePath(const std::string & thePath, const std::string & theAppPath) {    
        includePaths_.push_back("sdcard/"+ theAppPath + "/" + thePath); 
        includePaths_.push_back("assets/" + thePath); 
    }
    
    std::string
    AndroidAssetProvider::getStringFromFile(const std::string & theFileName) const {
        if (theFileName.size() > 0 ) {
            //unzipped, read from sdcard
            std::string myContent;
            std::string filePath;
            if (masl::searchFile(includePaths_, theFileName, filePath)) {
                masl::readFile(filePath, myContent);
                return myContent;
            }
        }
        return readFromPackage(includePaths_, _myApkArchive, theFileName);
    }

    std::vector<char>
    AndroidAssetProvider::getBlockFromFile(const std::string & theFileName) const {
        if (theFileName.size() > 0) {
            //unzipped, read from sdcard
            std::vector<char> myContent;
            std::string filePath;
            if (masl::searchFile(includePaths_, theFileName, filePath)) {
                masl::readBinaryFile(filePath, myContent);
                return myContent;
            }
        }
        return readBinaryFromPackage(includePaths_, _myApkArchive, theFileName);
    }

    std::vector<std::string>
    AndroidAssetProvider::getLineByLineFromFile(const std::string & theFileName) const {
        if (theFileName.size() > 0) {
            //unzipped, read from sdcard
            std::vector<std::string> myContent;
            std::string filePath;
            if (masl::searchFile(includePaths_, theFileName, filePath)) {
                masl::readFileLineByLine(filePath, myContent);
                return myContent;
            }
        }
        return readLineByLineFromPackage(includePaths_, _myApkArchive, theFileName);
    }

    bool
    AndroidAssetProvider::loadTextureFromPNG(const std::string & theFileName, unsigned int & textureId, int & width, int & height, bool & rgb) {
        std::string myFilename = masl::trimall(theFileName);
        if (myFilename.size() > 0) {
            //unzipped, read from sdcard
            std::string filePath;
            if (masl::searchFile(includePaths_, myFilename, filePath)) {
                return mar::loadTextureFromPNG(filePath, textureId, width, height, rgb);
            }
        }
        return android::loadTextureFromPNG(_myApkArchive, myFilename, textureId, width, height, rgb);
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
    
    std::string AndroidAssetProvider::getDownloadPath() const {
        return getAssetPath() + "/downloads/";
    }
    
}

