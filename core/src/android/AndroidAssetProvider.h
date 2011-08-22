#ifndef _included_mobile_android_AndroidPackageProvider_
#define _included_mobile_android_AndroidPackageProvider_

#include <string>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <libzip/zip.h>

#include <mar/AssetProvider.h>

namespace android {
    class AndroidAssetProvider : public mar::AssetProvider {
    public:
        AndroidAssetProvider(const std::string & theApkPath);
        virtual ~AndroidAssetProvider();

        virtual std::string getStringFromFile(const std::string & theFileName) const;        

        zip* getAPK() { return _myApkArchive;};

    private:
        zip * _myApkArchive;
    };

    typedef boost::shared_ptr<AndroidAssetProvider> AndroidAssetProviderPtr;
};

#endif
