// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _SINGLETON_BASE_INCLUDED_
#define _SINGLETON_BASE_INCLUDED_


namespace masl {

    class  SingletonBase {
    public:
        // The Singleton Manager will call this method on all singletons before it starts deleting them.
        // This way singletons can correctly shut down, independent of the order of destruction.
        virtual void stop() {};

        virtual ~SingletonBase() {}
    };

}

#endif
