#include "AndroidAssetProvider.h"

#include <masl/Logger.h>

#include "APK_functions.h"
#include "png_functions.h"

namespace android {

    AndroidAssetProvider::AndroidAssetProvider(const std::string & theApkPath) 
        : AssetProvider(), _myApkArchive(NULL) 
    {
        android::loadAPK(&_myApkArchive, theApkPath);
    }

    AndroidAssetProvider::~AndroidAssetProvider() {
        //delete _myApkArchive; //warnings?
    }

    std::string AndroidAssetProvider::getStringFromFile(const std::string & theFileName) const {
        return readFromPackage(_myApkArchive, theFileName);
    }

    std::vector<std::string> AndroidAssetProvider::getLineByLineFromFile(const std::string & theFileName) const
    {
        return readLineByLineFromPackage(_myApkArchive, theFileName);
    }

    bool AndroidAssetProvider::loadTextureFromPNG(const std::string & filename, GLuint & textureId, int & width, int & height, bool & rgb) {
        return android::loadTextureFromPNG(_myApkArchive, filename, textureId, width, height, rgb);
    }
}

