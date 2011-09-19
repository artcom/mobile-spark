#ifndef _SINGLETON_BASE_INCLUDED_
#define _SINGLETON_BASE_INCLUDED_


namespace masl {

    class  SingletonBase {
    public:
        // The Singleton Manager will call this method on all singletons before it starts deleting them.
        // This way singletons can correctly shut down, independent of the order of destruction.
        virtual void stop() {};

        virtual ~SingletonBase() {}
    };

}

#endif
