/* __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2008, ART+COM AG Berlin, Germany <www.artcom.de>
//
// This file is part of the ART+COM Standard Library (asl).
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
//
// Description:  Automatic Resource Management Functions
//
// Last Review: pavel 30.11.2005
//
//  review status report: (perfect, ok, fair, poor, disaster)
//    usefullness            : ok
//    formatting             : ok
//    documentation          : poor
//    test coverage          : poor
//    names                  : fair
//    style guide conformance: ok
//    technical soundness    : ok
//    dead code              : ok
//    readability            : ok
//    understandabilty       : fair
//    interfaces             : ok
//    confidence             : ok
//    integration            : ok
//    dependencies           : ok
//    cheesyness             : ok
//
//    overall review status  : fair
//
//    recommendation:
//       - improve documentation
//       - improve test coverage
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
*/

#ifndef _included_masl_Auto_
#define _included_masl_Auto_

//#include "asl_base_settings.h"

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

