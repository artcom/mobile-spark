#ifndef _SINGLETON_MANAGER_INCLUDED_
#define _SINGLETON_MANAGER_INCLUDED_


#include <iostream>
#include <string>
#include <map>
#include <list>
#include <typeinfo>

#ifdef iOS
    //iOS
    #include <iostream>
#elif ANDROID
    //Android
    #include <android/log.h>
#endif

#include <boost/pointer_cast.hpp>

#include "Ptr.h"
#include "Exception.h"
#include "SingletonBase.h"
#include "string_functions.h"

namespace masl {

/** Singletonmanager - The Why and How
 *
 * Dynamically linking plug-ins to our process breaks the classic
 * Singleton Pattern (see "Effective C++") if these Plug-Ins are statically linked
 * with the same base libraries that are also used in the main binary.
 * This is because duplicating the library code also duplicates local static variables
 * which are typically used to ensure that Singletons are instantiated only once.
 * This problem is solved by passing a pointer to a SingletonManager to each
 * dynamically loaded module at load time. This SingletonManager, which is a classic Singleton
 * and therefore exists once per module, is then used as a delegate for the module-local
 * SingletonManager instance. To put it another way: Each module has its own SingletonManager that
 * forwards its calls to main module's Singletonmanager instances. -jbo
 * WARNING: When the SingletonManager is destroyed, it destructs all Singletons in reverse
 * order of construction. Make sure no Singletons are accessed after the SingletonManager
 * was destroyed!
 */

class SingletonManager {
public:
    ~SingletonManager();

    // This function is called by the destructor. It is also called by the windows ctrl-break
    // handler to make sure that all singletons are destroyed even if the (static)
    // SingletonManager isn't.
    void destroyAllSingletons();

    // The SingletonManager itself is a classic Singleton
    // (Under Windows it will be instantiated once per module/dll/plugin)
    static SingletonManager & get();

    // needs to be called by each dynamically loaded Plug-In
    // to delegate singleton lookup to the one and only
    // SingletonManager in charge for the running process.
    void setDelegate(SingletonManager * theDelegate);

    template<class T>
    T& getSingletonInstance() const {
        //__android_log_print(ANDROID_LOG_WARN, "SingletonManager type: ", typeid(T).name());
        // NOTE: this is only called once per module and Singleton class,
        // so performance doesn't really matter.
        if (_myDelegate) {
            return _myDelegate->getSingletonInstance<T>();
        }
        SingletonMap::iterator i = _mySingletonMap.find(&typeid(T));

        if (i != _mySingletonMap.end()) {
            return *( boost::dynamic_pointer_cast<T>(i->second));
        } else {
            SingletonBase * sb = new T();
            masl::Ptr<SingletonBase> mySingletonPtr = masl::Ptr<SingletonBase>(sb);
            _mySingletonList.push_back(mySingletonPtr);

            _mySingletonMap.insert(std::make_pair(&typeid(T), mySingletonPtr));
            return * boost::dynamic_pointer_cast<T>(sb);
        }
    }
private:
    SingletonManager() :
        _myDelegate(0)
    {}

    class LessTypeInfoPtr {
    public:
        bool operator()(const std::type_info *theLeft, const std::type_info *theRight) const {
            return 0 != theLeft->before(*theRight);
        }
    };

    // don't even think of copying this ;)
    SingletonManager(const SingletonManager&) {}
    SingletonManager & operator=(const SingletonManager &) {return *this;}

    SingletonManager *_myDelegate;

    typedef std::map< const std::type_info *, masl::Ptr<SingletonBase>, LessTypeInfoPtr> SingletonMap;
    typedef std::list< masl::Ptr<SingletonBase> > SingletonList;

    mutable SingletonList _mySingletonList; // for keeping track of order of construction
    mutable SingletonMap _mySingletonMap;   // for fast lookup
};


/** SingletonDeleter
 *
 * When using dynamic linking (DLLs) in Windows, DLLs are loaded and unloaded
 * in some order that the OS determines. Therefor it might happen that a DLL
 * already is unloaded when the SingletonManager, from its destructor, tries
 * to call code in that DLL in order to destroy a singleton.
 *
 * To prevent this, the global constant singletonDeleter instance, instantiated
 * once for every DLL, will destroy all singletons from its destructor as soon
 * as the first DLL is unloaded.
 */
class SingletonDeleter {
public:
    SingletonDeleter() {}
    ~SingletonDeleter()
    {
        masl::SingletonManager::get().destroyAllSingletons();
    }
private:
    SingletonDeleter(const SingletonDeleter&);
    SingletonDeleter& operator=(const SingletonDeleter&);
};

const SingletonDeleter ourSingletonDeleter;

}

#endif
