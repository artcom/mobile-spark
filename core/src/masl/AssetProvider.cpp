// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

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
        if (masl::searchFile(includePaths_, theFilename, filePath)) {
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

