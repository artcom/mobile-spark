#include "APK_functions.h"

#include <masl/Logger.h>

#define DB(x) //x

using namespace std;
namespace android {

    DEFINE_EXCEPTION(APKLoadingException, masl::Exception)

    void loadAPK (zip** theAPKArchive, const string & apkPath) {
        AC_INFO << "Loading APK " << apkPath;
        *theAPKArchive = zip_open(apkPath.c_str(), 0, NULL);
        if (!(*theAPKArchive)) {
            AC_ERROR << "Error loading APK " << apkPath;
            throw APKLoadingException("Error loading APK " + apkPath, PLUS_FILE_LINE);
            return;
        }

        ////Just for debug, print APK contents
        DB(int numFiles = zip_get_num_files(*theAPKArchive);
            for (int i=0; i<numFiles; i++) {
                const char* name = zip_get_name(*theAPKArchive, i, 0);
                if (name == NULL) {
                    AC_ERROR << "Error reading zip file name at index " << zip_strerror(*theAPKArchive);
                    return;
                }
                AC_PRINT << "File " << i >> ": " << name;
            }
        )
    }

    std::string readFromPackage(const std::vector<std::string> & theIncludeList, 
                                zip* theAPKArchive, const string &  theFileName) {
        std::string content = "";
        const size_t MAX_LENGTH = 50000;
        zip_file* file = NULL;
        bool foundFile = searchFile(theIncludeList, theAPKArchive, theFileName, file, true);
        if (!foundFile) {
            return NULL;
        }
        char buffer[MAX_LENGTH];
        size_t size = zip_fread(file, buffer, MAX_LENGTH);
        content = std::string(buffer, size);
        zip_fclose(file);
        return content;
    }

    std::vector<char> readBinaryFromPackage(const std::vector<std::string> & theIncludeList,
                                            zip* theAPKArchive, const string &  theFileName) {
        std::vector<char> content;
        const size_t MAX_LENGTH = 5000;
        zip_file* file = NULL;
        bool foundFile = searchFile(theIncludeList, theAPKArchive, theFileName, file, true);
        if (!foundFile) {
            return content;
        }
        char buffer[MAX_LENGTH];
        size_t size = zip_fread(file, buffer, MAX_LENGTH);
        copy(buffer, buffer + size, back_inserter(content));
        zip_fclose(file);
        return content;
    }

    std::vector<std::string> readLineByLineFromPackage(const std::vector<std::string> & theIncludeList,
                                                       zip* theAPKArchive, const string & theFileName) {
        std::vector<std::string> content;
        const size_t MAX_LENGTH = 1000;
        char buffer[MAX_LENGTH];
        std::string newPart;
        zip_file* file = NULL;
        bool foundFile = searchFile(theIncludeList, theAPKArchive, theFileName, file, true);
        if (!foundFile) {
            return content;
        }
        size_t size = zip_fread(file, buffer, MAX_LENGTH);
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
                }
                first = false;
            }
            endedWithNewLine = (item.size() == 0);
            size = zip_fread(file, buffer, MAX_LENGTH);
        }
        zip_fclose(file);
        return content;
    }
 
    bool searchFile(const std::vector<std::string> & theIncludeList, zip* theAPKArchive, 
                    const string & theFileName, zip_file* & file, const bool theForce) {
        for (std::vector<std::string>::const_iterator it = theIncludeList.begin(); it != theIncludeList.end(); ++it) {
            if (searchFile(theAPKArchive, (*it) + theFileName, file, false)) {                
                return true;
            }
        }
        return false;
    }
    
    //if file exists, returns reference to zip_file which has to be zip_fclosed by caller
    bool searchFile(zip* theAPKArchive, const string & theFileName, zip_file* & file, const bool theForce) {
        string myFilename = theFileName;
        if (!theAPKArchive) {
            AC_ERROR << "apk broken";
            throw APKLoadingException("apk broken " + myFilename, PLUS_FILE_LINE);
        }
        file = zip_fopen(theAPKArchive, myFilename.c_str(), 0);
        if (!file) {
            if (theForce) {
                AC_ERROR << "Error opening " << myFilename << " from APK";
                throw APKLoadingException("Error opening APK " + myFilename, PLUS_FILE_LINE);
            }
            return false;
        }
        return true;
    }
}

