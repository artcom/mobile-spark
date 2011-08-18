#include "mar/AssetProvider.h"
#include "masl/file_functions.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#import <Foundation/NSString.h>
#import <Foundation/NSBundle.h>
#import <Foundation/NSScanner.h>

class IOSAssetProvider : public mar::AssetProvider {
public:
    IOSAssetProvider(const std::string & theAssetFolderPath);
    virtual ~IOSAssetProvider();
    virtual std::string getStringFromFile(const std::string & theFile) const = 0;
private:
    const std::string &_myAssetFolderPath;
};

typedef boost::shared_ptr<IOSAssetProvider> IOSAssetProviderPtr;


