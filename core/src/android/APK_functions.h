// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_android_apk_functions_
#define _included_mobile_android_apk_functions_

#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <libzip/zip.h>

#include <masl/Exception.h>

namespace android {

    DECLARE_EXCEPTION(APKLoadingException, masl::Exception)

    void loadAPK (zip** theAPKArchive, const std::string & apkPath);
    std::string readFromPackage(zip* theAPKArchive, const std::string & theFileName);
    std::vector<char> readBinaryFromPackage(zip* theAPKArchive, const std::string & theFileName);
    std::vector<std::string> readLineByLineFromPackage(zip* theAPKArchive, const std::string & theFileName);
    bool searchFile(zip* theAPKArchive, const std::string & theFileName, zip_file* file, const bool theForce = false);
    void getDirectoryEntries(zip* theAPKArchive, const std::string & thePath, std::vector<std::string> & theDirEntries, const std::string & theFilter);

};

#endif
