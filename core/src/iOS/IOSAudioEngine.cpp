#include "IOSAudioEngine.h"


namespace ios
{
    IOSAudioEngine::IOSAudioEngine()
    : AssetProvider()
    {
    }

    IOSAssetProvider::~IOSAssetProvider()
    {
    }

    unsigned int
    IOSAssetProvider::playEffect(const std::string & theFile) const {
        std::string myFoundFile = masl::AssetProviderSingleton::get().ap()->findFile(theFile);
        AC_DEBUG << "play Effect " << myFoundFile;
		int ret = 0;
        //TODO
		return (unsigned int)ret;
    }

}
