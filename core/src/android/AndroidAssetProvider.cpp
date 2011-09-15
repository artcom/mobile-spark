#include "AndroidAssetProvider.h"

#include <masl/Logger.h>
#include <masl/file_functions.h>
#include <mar/png_functions.h>

#include "APK_functions.h"
#include "png_functions.h"

using namespace std;
namespace android {

    AndroidAssetProvider::AndroidAssetProvider(const std::string & theApkPath) 
        : AssetProvider(), _myApkArchive(NULL) 
    {
        AC_PRINT << "AndroidAssetProvider ctor";
        android::loadAPK(&_myApkArchive, theApkPath);
        AC_PRINT << "AndroidAssetProvider ctor end";
    }

    AndroidAssetProvider::~AndroidAssetProvider() {
        //delete _myApkArchive; //warnings?
    }
    
    string 
    AndroidAssetProvider::findFile(const std::string & theFilename) const{
        std::string filePath;
        if (masl::searchFile(includePaths_, theFilename, filePath)) {
            return filePath;
        } else {
            return "";
        }        
    }

    vector<string> 
    AndroidAssetProvider::getFilesFromPath(const string & theBasename) const {
        vector<string> myFiles;
        if (theBasename.size() > 0 && theBasename[0] == '/') {  
            for (std::vector<std::string>::const_iterator it = includePaths_.begin(); it != includePaths_.end(); ++it) {            
                masl::getDirectoryEntries((*it), myFiles, theBasename.substr(1, theBasename.size()));
            }
        }
        return myFiles;
    }

    std::string 
    AndroidAssetProvider::getStringFromFile(const std::string & theFileName) const {
        if (theFileName.size() > 0 && theFileName[0] == '/') {  
            //unzipped, read from sdcard
            std::string myContent;
            std::string filePath;
            if (masl::searchFile(includePaths_, theFileName, filePath)) {
                masl::readFile(filePath, myContent);
            }
            return myContent;
        }
        return readFromPackage(_myApkArchive, theFileName);
    }

    std::vector<std::string> 
    AndroidAssetProvider::getLineByLineFromFile(const std::string & theFileName) const {
        if (theFileName.size() > 0 && theFileName[0] == '/') {  
            //unzipped, read from sdcard
            std::vector<std::string> myContent;
            std::string filePath;
            if (masl::searchFile(includePaths_, theFileName, filePath)) {
                masl::readFileLineByLine(filePath, myContent);
            }
            return myContent;
        } 
        return readLineByLineFromPackage(_myApkArchive, theFileName);
    }

    bool 
    AndroidAssetProvider::loadTextureFromPNG(const std::string & theFileName, GLuint & textureId, int & width, int & height, bool & rgb) {
        std::string myFilename = masl::trimall(theFileName);        
        if (myFilename.size() > 0 && myFilename[0] == '/') {
            //unzipped, read from sdcard
            std::string filePath;
            if (masl::searchFile(includePaths_, myFilename, filePath)) {
                return mar::loadTextureFromPNG(filePath, textureId, width, height, rgb);
            }
        }
        return android::loadTextureFromPNG(_myApkArchive, myFilename, textureId, width, height, rgb);
    }
}

