#include "mar/AssetProvider.h"
#include "masl/file_functions.h"
#include <boost/smart_ptr/shared_ptr.hpp>

namespace ios {
class IOSAssetProvider : public mar::AssetProvider {
public:
    IOSAssetProvider(const std::string & theAssetFolderPath);
    virtual ~IOSAssetProvider();
    virtual std::string getStringFromFile(const std::string & theFile) const;
private:
    const std::string &_myAssetFolderPath;
};
typedef boost::shared_ptr<IOSAssetProvider> IOSAssetProviderPtr;
};

