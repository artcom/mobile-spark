#include "AssetProvider.h"
#include "file_functions.h"

namespace masl {
    AssetProviderSingleton::~AssetProviderSingleton() {}

    void
    AssetProviderSingleton::setAssetProvider(AssetProviderPtr theAssetProvider) {
        _myAssetProvider = theAssetProvider;
    };

    std::string
    AssetProvider::findFile(const std::string & theFilename) const{
        std::string filePath;
        if (masl::searchFile(theFilename, filePath)) {
            return filePath;
        } else {
            return "";
        }
    }

    //takes relative Path
    std::vector<std::string>
    AssetProvider::getFilesFromPath(const std::string & thePath, const std::string & thePattern, const bool theForceOnlyInBundle_APK) const {
        std::vector<std::string> myFiles;
        masl::getDirectoryEntries(getAssetPath() + "/" + thePath, myFiles, thePattern);
        return myFiles;
    }
};

