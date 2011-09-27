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
// Description: process & system memory related functions
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
*/

#ifndef _included_mobile_masl_proc_functions_
#define _included_mobile_masl_proc_functions_


#include <sys/types.h>

namespace masl {

    /// Get amount of system memory in KB.
    unsigned getTotalMemory();

    /// Get amount of used system memory in KB.
    unsigned getUsedMemory();

    /// Get amount of free system memory in KB.
    unsigned getFreeMemory();

    /**
     * Get process memory usage in KB.
     * @param thePid process ID, default is to get for own process.
     */
    typedef pid_t ProcessID;
    unsigned getProcessMemoryUsage(ProcessID thePid = 0);

}

#endif

