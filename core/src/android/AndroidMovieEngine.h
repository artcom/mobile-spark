// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_android_AndroidMovieEngine_
#define _included_mobile_android_AndroidMovieEngine_

#include <masl/MovieEngine.h>

namespace android {

    class AndroidMovieEngine : public masl::MovieEngine {
    public:
        AndroidMovieEngine(const std::string & theJavaActivity);
        virtual ~AndroidMovieEngine();

        void playMovie(void* theMovieWidget, const std::string & theSrc);
        void stopMovie(void* theMovieWidget);
        void pauseMovie(void* theMovieWidget);
        void resetMovie(void* theMovieWidget);
        void updateMovieTexture(void* theMovieWidget);
        const masl::VideoInfo getMovieInfo(void* theMovieWidget) const;
        bool isMoviePlaying(void* theMovieWidget) const;
        void setMovieVolume(void* theMovieWidget, float newVolume);

    private:
        typedef std::map<void*, std::string> MovieMap;
        MovieMap _movieMap;
        
        std::string javaActivity_;

    };
    typedef masl::Ptr<AndroidMovieEngine> AndroidMovieEnginePtr;
};

#endif
