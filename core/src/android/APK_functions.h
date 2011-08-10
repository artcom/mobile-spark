#ifndef _included_mobile_android_apk_functions_
#define _included_mobile_android_apk_functions_

#include <map>
#include <vector>
#include <sstream>
#include <zip.h>

#include <boost/smart_ptr/shared_ptr.hpp>


/* collection of static helpers */

namespace android {

    void loadAPK (zip** theAPKArchive, const char* apkPath);
    std::string readFromFile(const std::string thePath);
    std::string readFromPackage(zip* theAPKArchive, const char* theFileName);
    std::vector<std::string> readLineByLineFromPackage(zip* theAPKArchive, const char* theFileName);
};

#endif 
