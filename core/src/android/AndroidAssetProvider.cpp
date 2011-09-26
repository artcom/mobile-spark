#include "AndroidAssetProvider.h"

#include <masl/Logger.h>
#include <masl/file_functions.h>
#include <mar/png_functions.h>

#include "APK_functions.h"
#include "png_functions.h"

using namespace std;
namespace android {

    AndroidAssetProvider::AndroidAssetProvider(const std::string & theApkPath)
        : AssetProvider(), _myApkArchive(NULL)
    {
        android::loadAPK(&_myApkArchive, theApkPath);
    }

    AndroidAssetProvider::~AndroidAssetProvider() {
        //delete _myApkArchive; //warnings?
    }

    std::string
    AndroidAssetProvider::getStringFromFile(const std::string & theFileName) const {
        if (theFileName.size() > 0 && theFileName[0] == '/') {
            //unzipped, read from sdcard
            std::string myContent;
            std::string filePath;
            if (masl::searchFile(includePaths_, theFileName, filePath)) {
                masl::readFile(filePath, myContent);
            } else {
                AC_ERROR << "file " << theFileName << " was not found in search paths";
                throw masl::FileNotFoundException("file " + theFileName + " was not found in search paths", PLUS_FILE_LINE);
            }
            return myContent;
        }
        return readFromPackage(_myApkArchive, theFileName);
    }

    std::vector<std::string>
    AndroidAssetProvider::getLineByLineFromFile(const std::string & theFileName) const {
        if (theFileName.size() > 0 && theFileName[0] == '/') {
            //unzipped, read from sdcard
            std::vector<std::string> myContent;
            std::string filePath;
            if (masl::searchFile(includePaths_, theFileName, filePath)) {
                masl::readFileLineByLine(filePath, myContent);
            } else {
                AC_ERROR << "file " << theFileName << " was not found in search paths";
                throw masl::FileNotFoundException("file " + theFileName + " was not found in search paths", PLUS_FILE_LINE);
            }
            return myContent;
        }
        return readLineByLineFromPackage(_myApkArchive, theFileName);
    }

    bool
    AndroidAssetProvider::loadTextureFromPNG(const std::string & theFileName, GLuint & textureId, int & width, int & height, bool & rgb) {
        std::string myFilename = masl::trimall(theFileName);
        if (myFilename.size() > 0 && myFilename[0] == '/') {
            //unzipped, read from sdcard
            std::string filePath;
            if (masl::searchFile(includePaths_, myFilename, filePath)) {
                return mar::loadTextureFromPNG(filePath, textureId, width, height, rgb);
            }
            AC_ERROR << "texture " << theFileName << " was not found in search paths";
            throw masl::FileNotFoundException("texture " + theFileName + " was not found in search paths", PLUS_FILE_LINE);
        }
        return android::loadTextureFromPNG(_myApkArchive, myFilename, textureId, width, height, rgb);
    }
}

