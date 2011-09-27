#ifndef _ac_mobile_masl_ThreadLock_h_included_
#define _ac_mobile_masl_ThreadLock_h_included_

#include <pthread.h>

namespace masl {

class ThreadLock {
    public:
        ThreadLock();
        virtual ~ThreadLock();

        /**
         * Locks (acquires a Mutex) for the Thread until the
         * ThreadLock is unlocked.
         * @warn the internal pthread_mutex has deadlock
         *       prevention enabled, hence a thread can not
         *       lock itself. Once it owns a ThreadLock, future
         *       calls to lock are ignored.
         */
        void lock();

        /**
         * tries to lock (acquire) the ThreadLock.
         * @return 0 if locked, EBUSY if it could not be locked
         */
        int nonblock_lock();
        /**
         * Unlocks (releases) the ThreadLock.
         */
        void unlock();


    private:
        ThreadLock (ThreadLock & otherLock);
        ThreadLock& operator= (ThreadLock & otherLock);

        pthread_mutex_t _myMutex;
};
}

#endif
