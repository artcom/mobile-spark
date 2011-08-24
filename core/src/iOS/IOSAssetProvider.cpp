#include "IOSAssetProvider.h"
namespace ios 
{
    IOSAssetProvider::IOSAssetProvider(const std::string & theAssetFolderPath)
    : AssetProvider(), _myAssetFolderPath(theAssetFolderPath)
    {
    }

    IOSAssetProvider::~IOSAssetProvider()
    {
    }

    bool IOSAssetProvider::loadTextureFromPNG(const std::string & filename, GLuint & textureId, int & width, int & height, bool & rgb)
    {
        return true;
    }
    
    
    std::string IOSAssetProvider::getStringFromFile(const std::string & theFile) const 
    {
        std::string content = "";
        masl::readFile(_myAssetFolderPath + "/" + theFile, content);
        return content;
    }

    std::vector<std::string> getLineByLineFromFile(const std::string & theFile) const
    {
        std::vector<std::string> myLines;
        //TODO
        return myLines;
    }
}
