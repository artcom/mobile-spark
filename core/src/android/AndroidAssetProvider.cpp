#include "AndroidAssetProvider.h"
#include "APK_functions.h"

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
}

