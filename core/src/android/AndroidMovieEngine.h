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
        virtual bool isAvailable();        

        void loadMovie(void* theMovieWidget, const std::string & theSrc);
        void playMovie(void* theMovieWidget);
        void stopMovie(void* theMovieWidget);
        void pauseMovie(void* theMovieWidget);
        void updateMovieTexture(void* theMovieWidget);
        const masl::VideoInfo getMovieInfo(void* theMovieWidget) const;
        bool isMoviePlaying(void* theMovieWidget) const;
        void setMovieVolume(void* theMovieWidget, const float newVolume);

    private:
        std::string javaActivity_;
    };
    typedef masl::Ptr<AndroidMovieEngine> AndroidMovieEnginePtr;
};

#endif
