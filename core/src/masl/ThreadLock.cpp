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
// Description: TODO
//
// Last Review: NEVER, NOONE
//
//  review status report: (perfect, ok, fair, poor, disaster, notapplicable, unknown)
//    usefullness            : unknown
//    formatting             : unknown
//    documentation          : unknown
//    test coverage          : unknown
//    names                  : unknown
//    style guide conformance: unknown
//    technical soundness    : unknown
//    dead code              : unknown
//    readability            : unknown
//    understandabilty       : unknown
//    interfaces             : unknown
//    confidence             : unknown
//    integration            : unknown
//    dependencies           : unknown
//    cheesyness             : unknown
//
//    overall review status  : unknown
//
//    recommendations:
//       - unknown
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
*/
//
//    $RCSfile: ThreadLock.cpp,v $
//
//     $Author: david $
//
//   $Revision: 1.6 $
//
// Description:
//
//=============================================================================

// own header
#include "ThreadLock.h"

#include "Logger.h"
#include <errno.h>
/*#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
*/

using namespace std;  // automatically added!
namespace masl {
void checkRetVal (int theRetVal, const char * theFunc) {
    if (theRetVal != 0) {
        static bool myIsInCheckRetVal = false;
        if (myIsInCheckRetVal) {
            // This happens if we're being called recursively and probably means that
            // AC_ERROR itself isn't working, so we make a feeble attempt to output
            // stuff anyway.
            cerr << "function="<<theFunc<<", error="<<strerror(theRetVal) << ", retVal= " << theRetVal << endl;
        } else {
            myIsInCheckRetVal = true;
            AC_ERROR << "function="<<theFunc<<", error="<<strerror(theRetVal) << ", retVal= " << theRetVal << endl;
            // Rumms. Besser, wir machten hier was sinnvolles.
            exit (-1);
        }
    }
}

ThreadLock::ThreadLock()
{
    pthread_mutex_t myMutexInitializer = PTHREAD_MUTEX_INITIALIZER;
    _myMutex = myMutexInitializer;
}

ThreadLock::~ThreadLock() {
}

void
ThreadLock::lock() {
    int myRetVal;
    myRetVal = pthread_mutex_lock (&_myMutex);
    checkRetVal (myRetVal, "pthread_mutex_lock");
}

int
ThreadLock::nonblock_lock() {
    int myRetVal;
    myRetVal = pthread_mutex_trylock (&_myMutex);
    if (myRetVal != EBUSY && myRetVal != 0) {
        checkRetVal (myRetVal, "pthread_mutex_trylock");
    }
    return myRetVal;
}

void
ThreadLock::unlock() {
    int myRetVal;
    // This causes an 'interrupted system call' error if the mutex isn't locked
    // before the call.
    myRetVal = pthread_mutex_unlock (&_myMutex);
    checkRetVal (myRetVal, "pthread_mutex_unlock");
}
}

