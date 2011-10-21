#ifndef _included_mobile_android_AndroidPackageProvider_
#define _included_mobile_android_AndroidPackageProvider_

#include <string>

#include <libzip/zip.h>

#include <masl/Ptr.h>
#include <masl/AssetProvider.h>

namespace android {
    class AndroidAssetProvider : public masl::AssetProvider {
    public:
        AndroidAssetProvider(const std::string & theApkPath, const std::string & theAppPath);
        virtual ~AndroidAssetProvider();

        virtual std::string getStringFromFile(const std::string & theFileName) const;
        virtual std::vector<char> getBlockFromFile(const std::string & theFileName) const;
        virtual std::vector<std::string> getLineByLineFromFile(const std::string & theFile) const;
        virtual bool loadTextureFromPNG(const std::string & filename, unsigned int & textureId, int & width, int & height, bool & rgb);
        virtual void addIncludePath(const std::string & thePath)  { includePaths_.push_back("sdcard/"+thePath); };
        virtual void storeInFile(const std::string & theFileName, const std::string & theData);
        virtual void storeInFile(const std::string & theFileName, const std::vector<char> & theData);
        virtual std::string getDownloadsPath() const;
        virtual std::string getDownloadsFolder() const;

        virtual std::vector<std::string> getFilesFromPath(const std::string & thePath, const std::string & thePattern = "", const bool theForceOnlyInBundle_APK = false) const;


    private:
        zip * _myApkArchive;
    };

    typedef masl::Ptr<AndroidAssetProvider> AndroidAssetProviderPtr;
};

#endif
