// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_masl_MovieEngine_h_included_
#define _ac_mobile_masl_MovieEngine_h_included_

#include <vector>

#include "Singleton.h"
#include "Logger.h"
#include "Ptr.h"

namespace masl {
    class MovieEngine {
    public:
        virtual bool playMovie(spark::MoviePtr theMovieWidget) = 0;
        virtual void stopMovie(spark::MoviePtr theMovieWidget) = 0;
        virtual void pauseMovie(spark::MoviePtr theMovieWidget) = 0;
        virtual void resetMovie(spark::MoviePtr theMovieWidget) = 0;
        virtual void updateMovieTexture(spark::MoviePtr theMovieWidget) = 0;
        virtual const VideoInfo getMovieInfo(spark::MoviePtr theMovieWidget) const = 0;
        virtual bool isMoviePlaying(spark::MoviePtr theMovieWidget) const = 0;
        virtual void setMovieVolume(spark::MoviePtr theMovieWidget, float newVolume) = 0;

    };
    typedef masl::Ptr<MovieEngine> MovieEnginePtr;

    class MovieEngineSingleton : public masl::Singleton<MovieEngineSingleton> {
    public:
        // we want a real singleton -> we need a non purevirtual out-of-line method other than dtor
        virtual void setMovieEngine(MovieEnginePtr theMovieEngine);
        const MovieEnginePtr & ae() const { return _myMovieEngine; };
    private:
        MovieEnginePtr _myMovieEngine;
    };
};
#endif
