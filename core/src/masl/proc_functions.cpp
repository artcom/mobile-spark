
#include "proc_functions.h"


#include "file_functions.h" // IO_Exception
#include "Logger.h"

using namespace std;

namespace masl {

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
            AC_ERROR << "unable to open " << myMemInfoFile;
            //throw IO_Failure("getMemInfo", std::string("Unable to open ") + myMemInfoFile);
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
            AC_ERROR << "unable to open " << myPidStatusFile;
            //throw IO_Failure("getProcMemInfo", std::string("Unable to open ") + myPidStatusFile);
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


    unsigned getTotalMemory() {
        static unsigned myTotalMemory = 0;
        if (myTotalMemory == 0) {
            myTotalMemory = getMemInfo(MEM_TOTAL) / 1024;
            AC_TRACE << "Total system memory " << (myTotalMemory / 1024) << " MB";
        }
        return myTotalMemory;
    }

    unsigned getUsedMemory() {
        unsigned myUsedMemory = 0;
        // Determining used/free memory under Linux is a bit tricky since
        // the kernel usually tries to take all free memory for caching.
        // Free memory is a waste of resources anyway...
        myUsedMemory = getMemInfo(MEM_USED) / 1024;
        AC_TRACE << "Used system memory " << (myUsedMemory / 1024) << " MB";
        return myUsedMemory;
    }

    unsigned getFreeMemory() {
        unsigned myFreeMemory = 0;
        myFreeMemory = getTotalMemory() - getUsedMemory();
        AC_TRACE << "Free system memory " << (myFreeMemory / 1024) << " MB";
        return myFreeMemory;
    }

    unsigned getProcessMemoryUsage(ProcessID thePid) {
        if (thePid == 0) {
            thePid = getpid();
        }
        unsigned myMemUsage = 0;
        myMemUsage = getProcMemInfo(thePid) / 1024;
        AC_TRACE << "Process " << thePid << " uses " << (myMemUsage / 1024) << " MB";
        return myMemUsage;
    }
}
