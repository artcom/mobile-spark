#ifndef _included_mobile_ios_AssetProvider_
#define _included_mobile_ios_AssetProvider_

#include <masl/Ptr.h>
#include <mar/AssetProvider.h>

namespace ios {
class IOSAssetProvider : public mar::AssetProvider {
public:
    IOSAssetProvider(const std::string & theAssetFolderPath);
    virtual ~IOSAssetProvider();
    virtual std::string getStringFromFile(const std::string & theFile) const;
    virtual std::vector<std::string> getLineByLineFromFile(const std::string & theFile) const;
    virtual bool loadTextureFromPNG(const std::string & filename, GLuint & textureId, int & width, int & height, bool & rgb);
    virtual void addIncludePath(const std::string & thePath)  { includePaths_.push_back(_myAssetFolderPath + "/"+thePath); };
private:
    const std::string _myAssetFolderPath;
};
typedef masl::Ptr<IOSAssetProvider> IOSAssetProviderPtr;
};

#endif
