// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

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

