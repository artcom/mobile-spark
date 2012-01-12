// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_Movie_
#define _included_mobile_spark_Movie_

#include "Image.h"

namespace spark {
    class Movie : public Image {
    public:
        Movie(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Movie();
        void play();   // start playback at playcursor
        void stop();   // stop videoplayback
        void pause();  // pause video and do not touch playcursor
        void reset();  // set playcursor to video start 
        
        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Movie::SPARK_TYPE;};
    protected:
    private:
        std::string moviesrc_;
    };

    typedef masl::Ptr<Movie> MoviePtr;
    typedef MemberFunctionEventCallback<Movie, MoviePtr> MovieCB;
};
#endif