// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "CallStack.h"

#define HAVE_DLADDR
#define HAVE_CXXABI

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef HAVE_DLADDR
#include <dlfcn.h>
#endif

#ifdef HAVE_CXXABI
#include <cxxabi.h>
#endif

#include <unwind.h>

#include "Logger.h"


/*****************************************************************************/
namespace masl {


typedef struct {
    size_t count;
    size_t ignore;
    const void** addrs;
    unsigned int lastAddr;
} stack_crawl_state_t;

static
_Unwind_Reason_Code trace_function(_Unwind_Context *context, void *arg)
{
    stack_crawl_state_t* state = (stack_crawl_state_t*)arg;
    if (state->count) {
        void* ip = (void*)_Unwind_GetIP(context);
        if (ip) {
            if (state->ignore) {
                state->ignore--;
            } else {
                state->addrs[0] = ip; 
                state->addrs++;
                state->count--;
            }
            if (state->lastAddr == (unsigned int)ip) {
                return _URC_END_OF_STACK;
            }
            state->lastAddr = (unsigned int)ip;
        }
    } else {
        return _URC_END_OF_STACK;
    }
    return _URC_NO_REASON;
}

static
int backtrace(const void** addrs, size_t ignore, size_t size)
{
    stack_crawl_state_t state;
    state.count = size;
    state.ignore = ignore;
    state.addrs = addrs;
#ifdef ANDROID
    _Unwind_Backtrace(trace_function, (void*)&state);
#endif
    return size - state.count;
}

/*****************************************************************************/

static 
const char *lookup_symbol(const void* addr, uint32_t *offset, char* name, size_t bufSize)
{
#ifdef HAVE_DLADDR
	Dl_info info;
	if (dladdr((void*)addr, &info)) {
		*offset = (uint32_t)info.dli_saddr;
		return info.dli_sname;
	}
#endif
	return NULL;
}

static 
int32_t linux_gcc_demangler(const char *mangled_name, std::string & unmangled_name, size_t buffersize)
{
	size_t out_len = 0;
#ifdef HAVE_CXXABI
	int status = 0;
	char *demangled = abi::__cxa_demangle(mangled_name, 0, &out_len, &status);
	if (status == 0) {
		// OK
		if (out_len < buffersize) {
            unmangled_name = demangled;
            out_len = unmangled_name.size();
        } else out_len = 0;
		free(demangled);
	} else {
		out_len = 0;
	}
#endif
	return out_len;
}

/*****************************************************************************/

class MapInfo {
    struct mapinfo {
        struct mapinfo *next;
        unsigned start;
        unsigned end;
        char name[];
    };

    const char *map_to_name(unsigned pc, const char* def) {
        mapinfo* mi = getMapInfoList();
        while(mi) {
            if ((pc >= mi->start) && (pc < mi->end))
                return mi->name;
            mi = mi->next;
        }
        return def;
    }

    mapinfo *parse_maps_line(char *line) {
        mapinfo *mi;
        int len = strlen(line);
        if (len < 1) return 0;
        line[--len] = 0;
        if (len < 50) return 0;
        if (line[20] != 'x') return 0;
        mi = (mapinfo*)malloc(sizeof(mapinfo) + (len - 47));
        if (mi == 0) return 0;
        mi->start = strtoul(line, 0, 16);
        mi->end = strtoul(line + 9, 0, 16);
        mi->next = 0;
        strcpy(mi->name, line + 49);
        return mi;
    }

    mapinfo* getMapInfoList() {
        //Mutex::Autolock _l(mLock);
        if (milist == 0) {
            char data[1024];
            FILE *fp;
            sprintf(data, "/proc/%d/maps", getpid());
            fp = fopen(data, "r");
            if (fp) {
                while(fgets(data, 1024, fp)) {
                    mapinfo *mi = parse_maps_line(data);
                    if(mi) {
                        mi->next = milist;
                        milist = mi;
                    }
                }
                fclose(fp);
            }
        }
        return milist;
    }
    mapinfo*    milist;
    //Mutex       mLock;
    static MapInfo sMapInfo;

public:
    MapInfo()
     : milist(0) {
    }

    ~MapInfo() {
        while (milist) {
            mapinfo *next = milist->next;
            free(milist);
            milist = next;
        }
    }
    
    static const char *mapAddressToName(const void* pc, const char* def) {
        return sMapInfo.map_to_name((unsigned)pc, def);
    }

};

/*****************************************************************************/

MapInfo MapInfo::sMapInfo;

/*****************************************************************************/

CallStack::CallStack()
    : mCount(0)
{
}

CallStack::CallStack(const CallStack& rhs)
    : mCount(rhs.mCount)
{
    if (mCount) {
        memcpy(mStack, rhs.mStack, mCount*sizeof(void*));
    }
}

CallStack::~CallStack()
{
}

CallStack& CallStack::operator = (const CallStack& rhs)
{
    mCount = rhs.mCount;
    if (mCount) {
        memcpy(mStack, rhs.mStack, mCount*sizeof(void*));
    }
    return *this;
}

bool CallStack::operator == (const CallStack& rhs) const {
    if (mCount != rhs.mCount)
        return false;
    return !mCount || (memcmp(mStack, rhs.mStack, mCount*sizeof(void*)) == 0);
}

bool CallStack::operator != (const CallStack& rhs) const {
    return !operator == (rhs);
}

bool CallStack::operator < (const CallStack& rhs) const {
    if (mCount != rhs.mCount)
        return mCount < rhs.mCount;
    return memcmp(mStack, rhs.mStack, mCount*sizeof(void*)) < 0;
}

bool CallStack::operator >= (const CallStack& rhs) const {
    return !operator < (rhs);
}

bool CallStack::operator > (const CallStack& rhs) const {
    if (mCount != rhs.mCount)
        return mCount > rhs.mCount;
    return memcmp(mStack, rhs.mStack, mCount*sizeof(void*)) > 0;
}

bool CallStack::operator <= (const CallStack& rhs) const {
    return !operator > (rhs);
}

const void* CallStack::operator [] (int index) const {
    if (index >= int(mCount))
        return 0;
    return mStack[index];
}


void CallStack::clear()
{
    mCount = 0;
}

void CallStack::update(int32_t ignoreDepth, int32_t maxDepth)
{
    if (maxDepth > MAX_DEPTH)
        maxDepth = MAX_DEPTH;
    mCount = backtrace(mStack, ignoreDepth, maxDepth);
}

// Return the stack frame name on the designated level
std::string CallStack::toStringSingleLevel(int32_t level) const
{
    std::string res;
    char namebuf[1024];
    //char tmp[256];
    std::string tmp;
    char tmp1[32];
    char tmp2[32];
    uint32_t offs;

    const void* ip = mStack[level];
    if (!ip) return res;

    snprintf(tmp1, 32, "#%02d  ", level);
    res.append(tmp1);

    const char* name = lookup_symbol(ip, &offs, namebuf, sizeof(namebuf));
    if (name) {
        if (linux_gcc_demangler(name, tmp, 256) != 0)
            name = tmp.c_str();
        snprintf(tmp1, 32, "0x%08x: <", (size_t)ip);
        snprintf(tmp2, 32, ">+0x%08x", offs);
        res.append(tmp1);
        res.append(name);
        res.append(tmp2);
    } else { 
        name = MapInfo::mapAddressToName(ip, "<unknown>");
        snprintf((char*)tmp.c_str(), 256, "pc %08x  %s", (size_t)ip, name);
        res.append(tmp);
    }
    return res;
}

// Dump a stack trace to the log
void CallStack::dump() const
{
    /* 
     * Sending a single long log may be truncated since the stack levels can
     * get very deep. So we request function names of each frame individually.
     */
    for (size_t i = 0; i < mCount; ++i) {
        AC_PRINT << toStringSingleLevel(i);
    }
}

// Return a string (possibly very long) containing the complete stack trace
std::ostream & operator<<(std::ostream & os, const CallStack & cs) {
    for (int i=0; i<int(cs.mCount); i++) {
        os << cs.toStringSingleLevel(i);
    }
    return os;
}

void dumpstack() {
    CallStack myStack;
    myStack.update();
    myStack.dump();
}

/*****************************************************************************/

}; // namespace 

