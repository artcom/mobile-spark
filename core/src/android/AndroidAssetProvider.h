#ifndef _included_mobile_android_AndroidPackageProvider_
#define _included_mobile_android_AndroidPackageProvider_

#include <string>

#include <libzip/zip.h>

#include <masl/Ptr.h>
#include <mar/AssetProvider.h>

namespace android {
    class AndroidAssetProvider : public mar::AssetProvider {
    public:
        AndroidAssetProvider(const std::string & theApkPath);
        virtual ~AndroidAssetProvider();

        virtual std::string getStringFromFile(const std::string & theFileName) const;
        virtual std::vector<std::string> getLineByLineFromFile(const std::string & theFile) const;
        virtual bool loadTextureFromPNG(const std::string & filename, GLuint & textureId, int & width, int & height, bool & rgb);
        virtual void addIncludePath(const std::string & thePath)  { includePaths_.push_back("sdcard/"+thePath); };

    private:
        zip * _myApkArchive;
    };

    typedef masl::Ptr<AndroidAssetProvider> AndroidAssetProviderPtr;
};

#endif
