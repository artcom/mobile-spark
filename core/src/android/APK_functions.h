#ifndef _included_mobile_android_apk_functions_
#define _included_mobile_android_apk_functions_

#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <libzip/zip.h>

#include <boost/smart_ptr/shared_ptr.hpp>

#include <masl/Exception.h>

namespace android {

    DECLARE_EXCEPTION(APKLoadingException, masl::Exception)

    void loadAPK (zip** theAPKArchive, const std::string & apkPath);
    std::string readFromPackage(zip* theAPKArchive, const std::string & theFileName);
    std::vector<std::string> readLineByLineFromPackage(zip* theAPKArchive, const std::string & theFileName);
};

#endif
