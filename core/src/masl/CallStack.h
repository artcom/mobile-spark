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

#ifndef ANDROID_CALLSTACK_H
#define ANDROID_CALLSTACK_H

#include <stdint.h>
#include <sys/types.h>

#include <string>

// ---------------------------------------------------------------------------

namespace masl {

class CallStack
{
public:
    enum {
        MAX_DEPTH = 31
    };

    CallStack();
    CallStack(const CallStack& rhs);
    ~CallStack();

    CallStack& operator = (const CallStack& rhs);
    
    bool operator == (const CallStack& rhs) const;
    bool operator != (const CallStack& rhs) const;
    bool operator < (const CallStack& rhs) const;
    bool operator >= (const CallStack& rhs) const;
    bool operator > (const CallStack& rhs) const;
    bool operator <= (const CallStack& rhs) const;
    
    const void* operator [] (int index) const;
    
    void clear();

    void update(int32_t ignoreDepth=0, int32_t maxDepth=MAX_DEPTH);

    // Dump a stack trace to the log
    void dump() const;

    friend std::ostream & operator<<(std::ostream & os, const CallStack & cs);
    
    size_t size() const { return mCount; }


private:
    // Internal helper function
    std::string toStringSingleLevel(int32_t level) const;

    size_t      mCount;
    const void* mStack[MAX_DEPTH];
};

    void dumpstack();

}; // namespace 


// ---------------------------------------------------------------------------

#endif 

