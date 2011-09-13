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

    bool IOSAssetProvider::loadTextureFromPNG(const std::string & filename, GLuint & textureId, int & outWidth, int & outHeight, bool & rgb)
    {
        return mar::loadTextureFromPNG(_myAssetFolderPath + "/" + filename, textureId, outWidth, outHeight, rgb);
    }
    
    
    std::string 
    IOSAssetProvider::getStringFromFile(const std::string & theFile) const {
        std::string content = "";
        if (theFile.size() > 0 && theFile[0] == '/') {  
            masl::readFile(_myAssetFolderPath + "/dataToPush/" + theFile, content);
            return content;
        }    
        masl::readFile(_myAssetFolderPath + "/" + theFile, content);
        return content;
    }


    std::vector<std::string> 
    IOSAssetProvider::getLineByLineFromFile(const std::string & theFile) const {
        std::vector<std::string> content;
        if (theFile.size() > 0 && theFile[0] == '/') {  
            masl::readFileLineByLine(_myAssetFolderPath + "/dataToPush/" + theFile, content);
            return content;
        }  
        masl::readFileLineByLine(_myAssetFolderPath + "/" + theFile, content);
        return content;
    }
}
