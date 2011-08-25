#include "AssetProvider.h"

namespace mar {
        
    AssetProviderSingleton::~AssetProviderSingleton() {}
    void 
    AssetProviderSingleton::setAssetProvider(AssetProviderPtr theAssetProvider) { 
        _myAssetProvider = theAssetProvider;
    };
};

