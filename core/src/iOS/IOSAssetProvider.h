#ifndef _included_mobile_ios_AssetProvider_
#define _included_mobile_ios_AssetProvider_

#include "mar/AssetProvider.h"
#include "masl/file_functions.h"
#include <boost/smart_ptr/shared_ptr.hpp>

namespace ios {
class IOSAssetProvider : public mar::AssetProvider {
public:
    IOSAssetProvider(const std::string & theAssetFolderPath);
    virtual ~IOSAssetProvider();
    virtual std::string getStringFromFile(const std::string & theFile) const;
    virtual std::vector<std::string> getLineByLineFromFile(const std::string & theFile) const;
    virtual bool loadTextureFromPNG(const std::string & filename, GLuint & textureId, int & width, int & height, bool & rgb);
private:
    const std::string &_myAssetFolderPath;
};
typedef boost::shared_ptr<IOSAssetProvider> IOSAssetProviderPtr;
};

#endif
