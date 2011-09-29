#ifndef _included_mobile_masl_proc_functions_
#define _included_mobile_masl_proc_functions_

#include <sys/types.h>

namespace masl {

    /// Get amount of system memory in KB.
    unsigned int getTotalMemory();

    /// Get amount of used system memory in KB.
    unsigned int getUsedMemory();

    /// Get amount of free system memory in KB.
    unsigned int getFreeMemory();

    /**
     * Get process memory usage in KB.
     * @param thePid process ID, default is to get for own process.
     */
    typedef pid_t ProcessID;
    unsigned int getProcessMemoryUsage(ProcessID thePid = 0);

}

#endif

