#include "IOSAssetProvider.h"
#include <masl/file_functions.h>
#include <mar/png_functions.h>

namespace ios 
{
    IOSAssetProvider::IOSAssetProvider(const std::string & theAssetFolderPath)
    : AssetProvider(), _myAssetFolderPath(theAssetFolderPath)
    {
    }

    IOSAssetProvider::~IOSAssetProvider()
    {
    }

    bool IOSAssetProvider::loadTextureFromPNG(const std::string & theFile, GLuint & textureId, int & outWidth, int & outHeight, bool & rgb)
    {
        if (theFile.size() > 0 && theFile[0] == '/') { 
            std::string filePath;
            if (masl::searchFile(includePaths_, theFile, filePath)) {
                return mar::loadTextureFromPNG(filePath, textureId, outWidth, outHeight, rgb);
            }
        }
        return mar::loadTextureFromPNG(_myAssetFolderPath + "/" + theFile, textureId, outWidth, outHeight, rgb);
    }
    
    
    std::string 
    IOSAssetProvider::getStringFromFile(const std::string & theFile) const {
        std::string content = "";
        if (theFile.size() > 0 && theFile[0] == '/') {  
            std::string filePath;
            if (masl::searchFile(includePaths_, theFile, filePath)) {
                masl::readFile(filePath, content);
            }
            return content;
        }    
        masl::readFile(_myAssetFolderPath + "/" + theFile, content);
        return content;
    }


    std::vector<std::string> 
    IOSAssetProvider::getLineByLineFromFile(const std::string & theFile) const {
        std::vector<std::string> content;
        if (theFile.size() > 0 && theFile[0] == '/') {  
            std::string filePath;
            if (masl::searchFile(includePaths_, theFile, filePath)) {
                masl::readFileLineByLine(filePath, content);
            }
            return content;
        }  
        masl::readFileLineByLine(_myAssetFolderPath + "/" + theFile, content);
        return content;
    }
}
