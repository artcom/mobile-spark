// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_ios_MovieBackend_
#define _included_mobile_ios_MovieBackend_

#import <AVFoundation/AVFoundation.h>

#include <masl/Ptr.h>

namespace ios {

    class MovieBackend 
    {
    public:
        MovieBackend();
        virtual ~MovieBackend();
        
        void playMovie(const std::string &theURL);
        
    private:
        
    };
    typedef masl::Ptr<MovieBackend> MovieBackendPtr;
};

#endif
