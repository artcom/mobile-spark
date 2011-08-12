#ifndef _ac_mobile_masl_AssetProvider_h_included_
#define _ac_mobile_masl_AssetProvider_h_included_

#include <boost/smart_ptr/shared_ptr.hpp>

namespace mar {
    class AssetProvider {
    public:
        virtual std::string getStringFromFile(const std::string & theFile) const = 0;
    };

    typedef boost::shared_ptr<AssetProvider> AssetProviderPtr;
};

#endif
