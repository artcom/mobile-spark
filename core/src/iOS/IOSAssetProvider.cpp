#import "IOSAssetProvider.h"
namespace ios {
IOSAssetProvider::IOSAssetProvider(const std::string & theAssetFolderPath)
: AssetProvider(), _myAssetFolderPath(theAssetFolderPath)
{
}

IOSAssetProvider::~IOSAssetProvider()
{
}

std::string IOSAssetProvider::getStringFromFile(const std::string & theFile) const 
{
    std::string content = "";
    masl::readFile(_myAssetFolderPath + "/" + theFile, content);
    return content;
}
}