#ifndef _ac_mobile_masl_Singleton_h_included_
#define _ac_mobile_masl_Singleton_h_included_

#include "SingletonManager.h"

namespace masl {

template<class T>
class Singleton : public SingletonBase {
public:
    static T& get() {
        // A Singleton caches the instance returned by SingletonManager
        // in a local static variable.
        static T& myInstance(SingletonManager::get().SingletonManager::getSingletonInstance<T>());
        return myInstance;
    }
};

}

#endif
