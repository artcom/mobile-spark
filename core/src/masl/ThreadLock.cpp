// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

// own header
#include "ThreadLock.h"

#include "Logger.h"
#include <errno.h>

namespace masl {
    
void
checkRetVal (int theRetVal, const char * theFunc) {
    if (theRetVal != 0) {
        static bool myIsInCheckRetVal = false;
        if (myIsInCheckRetVal) {
            // This happens if we're being called recursively and probably means that
            // AC_ERROR itself isn't working, so we make a feeble attempt to output
            // stuff anyway.
            std::cerr << "function="<<theFunc<<", error="<<strerror(theRetVal) << ", retVal= " << theRetVal << std::endl;
        } else {
            myIsInCheckRetVal = true;
            AC_ERROR << "function="<<theFunc<<", error="<<strerror(theRetVal) << ", retVal= " << theRetVal << std::endl;
            // Rumms. Besser, wir machten hier was sinnvolles.
            exit (-1); //XXX
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

