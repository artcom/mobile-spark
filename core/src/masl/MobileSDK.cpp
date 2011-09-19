#include "MobileSDK.h"

namespace masl {
    MobileSDK_Singleton::~MobileSDK_Singleton() {}

    void
    MobileSDK_Singleton::setMobileSDK(MobileSDKPtr theMobileSDK) {
        _myMobileSDK = theMobileSDK;
    }
}
