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

    bool searchFile(zip* theAPKArchive, const std::string & theFileName, std::string & retPath, const bool theForce = false);
    bool fileExist(zip* theAPKArchive, const std::string & theFileName);

    void getDirectoryEntries(zip* theAPKArchive, const std::string & thePath, std::vector<std::string> & theDirEntries, const std::string & theFilter);

};

#endif
