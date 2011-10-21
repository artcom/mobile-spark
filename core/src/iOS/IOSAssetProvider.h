#ifndef _included_mobile_ios_AssetProvider_
#define _included_mobile_ios_AssetProvider_

#include <masl/Ptr.h>
#include <masl/AssetProvider.h>

namespace ios {
class IOSAssetProvider : public masl::AssetProvider {
public:
    IOSAssetProvider(const std::string & theAssetFolderPath, const std::string & theAppPath);
    virtual ~IOSAssetProvider();
    virtual std::string getStringFromFile(const std::string & theFile) const;
    virtual std::vector<char> getBlockFromFile(const std::string & theFileName) const;
    virtual std::vector<std::string> getLineByLineFromFile(const std::string & theFile) const;
    virtual bool loadTextureFromPNG(const std::string & filename, unsigned int & textureId, int & width, int & height, bool & rgb);
    virtual void addIncludePath(const std::string & thePath)  { includePaths_.push_back(_myAssetFolderPath + "/"+thePath); };
    virtual void storeInFile(const std::string & theFileName, const std::string & theData);
    virtual void storeInFile(const std::string & theFileName, const std::vector<char> & theData);
    
    virtual std::string getDownloadPath() const;

private:
    const std::string _myAssetFolderPath;
};
typedef masl::Ptr<IOSAssetProvider> IOSAssetProviderPtr;
};

#endif
