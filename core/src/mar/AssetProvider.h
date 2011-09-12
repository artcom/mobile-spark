#ifndef _ac_mobile_masl_AssetProvider_h_included_
#define _ac_mobile_masl_AssetProvider_h_included_

#include <vector>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <masl/Singleton.h>
#include <masl/Logger.h>

#include "GlHeaders.h"

namespace mar {
    class AssetProvider {
    public:
        virtual std::string getStringFromFile(const std::string & theFile) const = 0;
        virtual std::vector<std::string> getLineByLineFromFile(const std::string & theFile) const = 0;
        virtual bool loadTextureFromPNG(const std::string & filename, GLuint & textureId, int & width, int & height, bool & rgb) = 0;
        virtual void addIncludePath(const std::string & thePath) = 0;
    protected:
        std::vector<std::string> includePaths_;
    };
    typedef boost::shared_ptr<AssetProvider> AssetProviderPtr;


    class AssetProviderSingleton : public masl::Singleton<AssetProviderSingleton> {
        public:
            virtual ~AssetProviderSingleton();
            void setAssetProvider(AssetProviderPtr theAssetProvider);
            const AssetProviderPtr & ap() const { return _myAssetProvider;};
    
        private:
            AssetProviderPtr _myAssetProvider;
    };

};

#endif
