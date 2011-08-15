#ifndef _included_mobile_android_apk_functions_
#define _included_mobile_android_apk_functions_

#include <map>
#include <vector>
#include <sstream>
#include <libzip/zip.h>

#include <boost/smart_ptr/shared_ptr.hpp>



namespace android {

    void loadAPK (zip** theAPKArchive, const char* apkPath);
    std::string readFromPackage(zip* theAPKArchive, const char* theFileName);
    std::vector<std::string> readLineByLineFromPackage(zip* theAPKArchive, const char* theFileName);
};

#endif 
