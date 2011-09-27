#ifndef _ac_mobile_masl_AutoLocker_h_included_
#define _ac_mobile_masl_AutoLocker_h_included_

namespace masl {
    template <class T>
    class AutoLocker {
        public:
            AutoLocker(T & theLock) : _myLock(theLock) {
                _myLock.lock();
            }

            ~AutoLocker() {
                _myLock.unlock();
            }

        private:
            T &  _myLock;
            AutoLocker(const AutoLocker&);
            AutoLocker& operator=(const AutoLocker&);
    };
}



#endif

