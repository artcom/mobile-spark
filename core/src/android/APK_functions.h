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
    std::string readFromPackage(const std::vector<std::string> & theIncludeList, zip* theAPKArchive, const std::string & theFileName);
    std::vector<char> readBinaryFromPackage(const std::vector<std::string> & theIncludeList, zip* theAPKArchive, const std::string & theFileName);
    std::vector<std::string> readLineByLineFromPackage(const std::vector<std::string> & theIncludeList, zip* theAPKArchive, const std::string & theFileName);

    bool searchFile(zip* theAPKArchive, const std::string & theFileName, zip_file* & file, const bool theForce = false);
    bool searchFile(const std::vector<std::string> & theIncludeList, zip* theAPKArchive, const std::string & theFileName, zip_file* & file, const bool theForce);
};

#endif
