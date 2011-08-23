#ifndef _ac_mobile_masl_AssetProvider_h_included_
#define _ac_mobile_masl_AssetProvider_h_included_

#include <boost/smart_ptr/shared_ptr.hpp>

#include "GlHeaders.h"

namespace mar {
    class AssetProvider {
    public:
        virtual std::string getStringFromFile(const std::string & theFile) const = 0;
        virtual bool loadTextureFromPNG(const std::string & filename, GLuint & textureId, int & width, int & height, bool & rgb) = 0;

    };

    typedef boost::shared_ptr<AssetProvider> AssetProviderPtr;
};

#endif
