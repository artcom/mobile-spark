#include "AssetProvider.h"
#include <masl/file_functions.h>
namespace mar {

    AssetProviderSingleton::~AssetProviderSingleton() {}

    void
    AssetProviderSingleton::setAssetProvider(AssetProviderPtr theAssetProvider) {
        _myAssetProvider = theAssetProvider;
    };

    std::string
    AssetProvider::findFile(const std::string & theFilename) const{
        std::string filePath;
        if (masl::searchFile(includePaths_, theFilename, filePath)) {
            return filePath;
        } else {
            return "";
        }
    }

    std::vector<std::string>
    AssetProvider::getFilesFromPath(const std::string & theBasename) const {
        std::vector<std::string> myFiles;
        if (theBasename.size() > 0 && theBasename[0] == '/') {
            for (std::vector<std::string>::const_iterator it = includePaths_.begin(); it != includePaths_.end(); ++it) {
                masl::getDirectoryEntries((*it), myFiles, theBasename.substr(1, theBasename.size()));
            }
        }
        return myFiles;
    }
};

