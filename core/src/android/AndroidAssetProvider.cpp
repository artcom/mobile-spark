#include "AndroidAssetProvider.h"

#include <masl/Logger.h>
#include <masl/file_functions.h>

#include "APK_functions.h"
#include "png_functions.h"

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

    std::string 
    AndroidAssetProvider::getStringFromFile(const std::string & theFileName) const {
        if (theFileName.size() > 0 && theFileName[0] == '/') {  
            //unzipped, read from sdcard
            std::string myContent;
            masl::readFile(theFileName, myContent);
            return myContent;
        }
        return readFromPackage(_myApkArchive, theFileName);
    }

    std::vector<std::string> 
    AndroidAssetProvider::getLineByLineFromFile(const std::string & theFileName) const {
        if (theFileName.size() > 0 && theFileName[0] == '/') {  
            //unzipped, read from sdcard
            std::vector<std::string> myContent;
            masl::readFileLineByLine(theFileName, myContent);
            return myContent;
        } 
        return readLineByLineFromPackage(_myApkArchive, theFileName);
    }

    bool 
    AndroidAssetProvider::loadTextureFromPNG(const std::string & filename, GLuint & textureId, int & width, int & height, bool & rgb) {
        return android::loadTextureFromPNG(_myApkArchive, filename, textureId, width, height, rgb);
    }
}

