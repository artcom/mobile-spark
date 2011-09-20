#ifndef _ac_mobile_asl_Settings_h_included_
#define _ac_mobile_asl_Settings_h_included_

#if __APPLE__
    #include "TargetConditionals.h"
    #ifdef TARGET_OS_IPHONE
         // iOS
    #elif TARGET_IPHONE_SIMULATOR
        // iOS Simulator
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
    #else
        // Unsupported platform
    #endif

#endif

namespace masl {
    typedef unsigned long long UInt64;
}

#endif
