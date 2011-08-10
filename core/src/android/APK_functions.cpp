#include "APK_functions.h"

#include <Logger.h>


namespace android {

    void loadAPK (zip** theAPKArchive, const char* apkPath) {
        AC_PRINT << "Loading APK " << apkPath;
        *theAPKArchive = zip_open(apkPath, 0, NULL);
        if (!(*theAPKArchive)) {
            AC_ERROR << "Error loading APK";
            return;
        }

        ////Just for debug, print APK contents
        //int numFiles = zip_get_num_files(*theAPKArchive);
        //
        //for (int i=0; i<numFiles; i++) {
        //    const char* name = zip_get_name(*theAPKArchive, i, 0);
        //    if (name == NULL) {
        //        AC_ERROR << "Error reading zip file name at index " << zip_strerror(*theAPKArchive);
        //        return;
        //    }
        //    AC_PRINT << "File " << i >> ": " << name;
        //}
    }

    std::string readFromPackage(zip* theAPKArchive, const char* theFileName) {
        std::string content = "";
        const size_t MAX_LENGTH = 5000;
        if (!theAPKArchive) {
            AC_ERROR << "apk broken";
        }
        zip_file* file = zip_fopen(theAPKArchive, theFileName, 0);
        if (!file) {
            AC_ERROR << "Error opening " << theFileName << " from APK";
            return NULL;
        }
        char buffer[MAX_LENGTH];
        size_t size = zip_fread(file, buffer, MAX_LENGTH);
        content = std::string(buffer, size);
        zip_fclose(file);
        return content;
    }

    std::vector<std::string> readLineByLineFromPackage(zip* theAPKArchive, const char* theFileName) {
        std::vector<std::string> content;
        const size_t MAX_LENGTH = 1000;
        char buffer[MAX_LENGTH];
        std::string newPart;

        if (!theAPKArchive) {
            AC_ERROR << "apk broken";
        }
        zip_file* file = zip_fopen(theAPKArchive, theFileName, 0);
        if (!file) {
            AC_ERROR << "Error opening " << theFileName << " from APK";
            return content;
        }
        size_t size = zip_fread(file, buffer, MAX_LENGTH);
        //AC_PRINT << " .... size " << size;
        bool endedWithNewLine = false;
        while (size > 0) {
            newPart = std::string(buffer, size);
            std::stringstream stream(newPart);
            std::string item;
            bool first = true;
            while (std::getline(stream, item, '\n')) {
                if (first && !endedWithNewLine && content.size() >0) {
                    content.back().append(item);
                } else {
                    content.push_back(item);
                    //AC_PRINT << "push " << item.c_str();
                }
                first = false;
            }
            endedWithNewLine = (item.size() == 0);
            size = zip_fread(file, buffer, MAX_LENGTH);
        }
        zip_fclose(file);
        return content;
    }
}

