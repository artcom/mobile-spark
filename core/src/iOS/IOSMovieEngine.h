// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_ios_IOSMovieEngine_
#define _included_mobile_ios_IOSMovieEngine_

#include <masl/MovieEngine.h>

namespace ios {
    class MovieController;
    typedef masl::Ptr<MovieController> MovieControllerPtr;

    class IOSMovieEngine : public masl::MovieEngine {
    public:
        IOSMovieEngine();
        virtual ~IOSMovieEngine();
        
        virtual bool isAvailable() { return true; }               
        void loadMovie(void* theMovieWidget, const std::string & theSrc);
        void playMovie(void* theMovieWidget);
        void stopMovie(void* theMovieWidget);
        void pauseMovie(void* theMovieWidget);
        void updateMovieTexture(void* theMovieWidget);
        const masl::VideoInfo getMovieInfo(void* theMovieWidget) const;
        bool isMoviePlaying(void* theMovieWidget) const;
        void setMovieVolume(void* theMovieWidget, const float newVolume);

    private:
        typedef std::map<void*, MovieControllerPtr> MovieMap;
        MovieMap _movieMap;
        
    };
    typedef masl::Ptr<IOSMovieEngine> IOSMovieEnginePtr;
};

#endif
