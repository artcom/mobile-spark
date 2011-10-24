// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

// own header
#include "proc_functions.h"

#include "Exception.h"
#include "Logger.h"

#ifdef iOS
#include <mach/mach.h>
#include <paths.h>
#include <sys/sysctl.h>
#endif

using namespace std;

namespace masl {

#ifdef ANDROID
    enum MemInfo {
        MEM_TOTAL,
        MEM_FREE,
        MEM_USED,
        MEM_USED_SYSTEM,
        SWAP_TOTAL,
        SWAP_FREE,
        SWAP_USED
    };

    unsigned getMemInfo(unsigned theMemInfo) {
        const char* myMemInfoFile = "/proc/meminfo";
        FILE* fp = fopen(myMemInfoFile, "r");
        if (!fp) {
            throw masl::Exception("getMemInfo", std::string("Unable to open ") + myMemInfoFile);
        }

        unsigned myMemTotal = 0, myMemFree = 0;
        unsigned myMemBuffers = 0, myMemCached = 0;
        unsigned mySwapTotal = 0, mySwapFree = 0;

        char myBuf[1024];
        char myKey[256];
        unsigned myValue;
        char myUnit[16];

        while (fgets(myBuf, sizeof(myBuf), fp)) {

            int n = sscanf(myBuf, "%s %d %s", myKey, &myValue, myUnit);
            if (n < 2) {
                continue;
            }

            if (n == 3) {
                unsigned myMemUnit = 1;
                if (strcasecmp(myUnit, "kb") == 0) {
                    myMemUnit = 1024;
                } else if (strcasecmp(myUnit, "mb") == 0) {
                    myMemUnit = 1024 * 1024;
                }
                myValue *= myMemUnit;
            }

            if (strcmp(myKey, "MemTotal:") == 0) {
                myMemTotal = myValue;
            } else if (strcmp(myKey, "MemFree:") == 0) {
                myMemFree = myValue;
            } else if (strcmp(myKey, "Buffers:") == 0) {
                myMemBuffers = myValue;
            } else if (strcmp(myKey, "Cached:") == 0) {
                myMemCached = myValue;
            } else if (strcmp(myKey, "SwapTotal:") == 0) {
                mySwapTotal = myValue;
            } else if (strcmp(myKey, "SwapFree:") == 0) {
                mySwapFree = myValue;
            }
        }
        fclose(fp);

        unsigned myMemInfo = 0;
        switch (theMemInfo) {
            case MEM_TOTAL:
                myMemInfo = myMemTotal;
                break;
            case MEM_FREE:
                myMemInfo = myMemFree;
                break;
            case MEM_USED:
                myMemInfo = myMemTotal - myMemFree - myMemBuffers - myMemCached;
                break;
            case MEM_USED_SYSTEM:
                myMemInfo = myMemBuffers + myMemCached;
                break;
            case SWAP_TOTAL:
                myMemInfo = mySwapTotal;
                break;
            case SWAP_FREE:
                myMemInfo = mySwapFree;
                break;
            case SWAP_USED:
                myMemInfo = mySwapTotal - mySwapFree;
                break;
        }
        return myMemInfo;
    }

    unsigned getProcMemInfo(pid_t thePid) {
        char myPidStatusFile[1024];
        snprintf(myPidStatusFile, sizeof(myPidStatusFile), "/proc/%d/status", thePid);
        FILE* fp = fopen(myPidStatusFile, "r");
        if (!fp) {
            throw masl::Exception("getProcMemInfo", std::string("Unable to open ") + myPidStatusFile);
        }

        unsigned myMemVirtual = 0, myMemPhysical = 0;
        unsigned myMemData = 0, myMemStack = 0, myMemExe = 0;

        char myBuf[1024];
        char myKey[256];
        unsigned myValue;
        char myUnit[16];

        while (fgets(myBuf, sizeof(myBuf), fp)) {

            int n = sscanf(myBuf, "%s %d %s", myKey, &myValue, myUnit);
            if (n < 2) {
                continue;
            }

            if (n == 3) {
                unsigned myMemUnit = 1;
                if (strcasecmp(myUnit, "kb") == 0) {
                    myMemUnit = 1024;
                } else if (strcasecmp(myUnit, "mb") == 0) {
                    myMemUnit = 1024 * 1024;
                }
                myValue *= myMemUnit;
            }

            if (strcmp(myKey, "VmSize:") == 0) {
                myMemVirtual = myValue;
            } else if (strcmp(myKey, "VmRSS:") == 0) {
                myMemPhysical = myValue;
            } else if (strcmp(myKey, "VmData:") == 0) {
                myMemData = myValue;
            } else if (strcmp(myKey, "VmStk:") == 0) {
                myMemStack = myValue;
            } else if (strcmp(myKey, "VmExe:") == 0) {
                myMemExe = myValue;
            }
        }
        fclose(fp);

        unsigned myMemInfo = 0;
        myMemInfo = myMemData + myMemStack + myMemExe;
        return myMemInfo;
    }
#endif

#ifdef iOS
    void getMemInfoMach(unsigned long long & theUsedBytes, unsigned long long & theFreeBytes) {
        mach_port_t myHostPort = mach_host_self();
        mach_msg_type_number_t myHostSize = sizeof(vm_statistics_data_t) / sizeof( integer_t);
        vm_size_t myPagesize;
        host_page_size(myHostPort, & myPagesize);
        vm_statistics_data_t myVmStats;
        if (host_statistics(myHostPort, HOST_VM_INFO, (host_info_t) & myVmStats,
                    & myHostSize) != KERN_SUCCESS)
        {
            throw masl::Exception("Failed to get memory info.", PLUS_FILE_LINE);
        }
        theUsedBytes = (myVmStats.active_count + myVmStats.inactive_count +
                myVmStats.wire_count) * myPagesize;
        theFreeBytes = myVmStats.free_count * myPagesize;
    }
#endif


    unsigned getTotalMemory() {
        static unsigned myTotalMemory = 0;
        if (myTotalMemory == 0) {
#ifdef ANDROID
            myTotalMemory = getMemInfo(MEM_TOTAL) / 1024;
#elif iOS
            unsigned long long myUsedBytes;
            unsigned long long myFreeBytes;
            getMemInfoMach( myUsedBytes, myFreeBytes);
            myTotalMemory = (myUsedBytes + myFreeBytes) / 1024;
#endif
            AC_TRACE << "Total system memory " << (myTotalMemory / 1024) << " MB";
        }
        return myTotalMemory;
    }

    unsigned getUsedMemory() {
        unsigned myUsedMemory = 0;
#ifdef ANDROID
        // Determining used/free memory under Linux is a bit tricky since
        // the kernel usually tries to take all free memory for caching.
        // Free memory is a waste of resources anyway...
        myUsedMemory = getMemInfo(MEM_USED) / 1024;
#elif iOS
        unsigned long long myUsedBytes;
        unsigned long long myFreeBytes;
        getMemInfoMach( myUsedBytes, myFreeBytes);
        myUsedMemory = myUsedBytes / 1024;
#endif
        AC_TRACE << "Used system memory " << (myUsedMemory / 1024) << " MB";
        return myUsedMemory;
    }

    unsigned getFreeMemory() {
        unsigned myFreeMemory = 0;
#ifdef ANDROID
        myFreeMemory = getTotalMemory() - getUsedMemory();
#elif iOS
        unsigned long long myUsedBytes;
        unsigned long long myFreeBytes;
        getMemInfoMach( myUsedBytes, myFreeBytes);
        myFreeMemory = myFreeBytes / 1024;
#endif
        AC_TRACE << "Free system memory " << (myFreeMemory / 1024) << " MB";
        return myFreeMemory;
    }

    unsigned getProcessMemoryUsage(ProcessID thePid) {
        if (thePid == 0) {
            thePid = getpid();
        } else {
#ifdef iOS
            throw masl::Exception("Can not retrieve process memory for foreign processes on MacOS X.",
                                 PLUS_FILE_LINE);
#endif
        }
        unsigned myMemUsage = 0;
#ifdef ANDROID
        myMemUsage = getProcMemInfo(thePid) / 1024;
#elif iOS
        kern_return_t myStatus;

        mach_port_t myTask;
        myStatus = task_for_pid(mach_task_self(), thePid, & myTask);
        if ( myStatus != KERN_SUCCESS) {
            AC_ERROR << "Mach call failed: " << mach_error_string(myStatus);
            return 0;
        }
        struct task_basic_info myTaskInfo;
        mach_msg_type_number_t myCount = TASK_BASIC_INFO_COUNT;
        myStatus = task_info(myTask, TASK_BASIC_INFO, (task_info_t)& myTaskInfo, & myCount);
        if ( myStatus != KERN_SUCCESS) {
            AC_ERROR << "Mach call failed: " << mach_error_string(myStatus);
            return 0;
        }

        // [DS] dunno exactly if this is correct ... but it works;
        myMemUsage = myTaskInfo.resident_size / 1024;
#endif
        AC_TRACE << "Process " << thePid << " uses " << (myMemUsage / 1024) << " MB";
        return myMemUsage;
    }
}
