// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

//Copyright (c) 2010-2011 cocos2d-x.org
//Copyright (c) 2009      On-Core
// 
//http://www.cocos2d-x.org
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in
//all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.

#ifndef _included_mobile_ios_IOSMovieEngine_
#define _included_mobile_ios_IOSMovieEngine_

#include <masl/MovieEngine.h>
#include "MovieController.h"

namespace ios {
    class IOSMovieEngine : public masl::MovieEngine {
    public:
        IOSMovieEngine();
        virtual ~IOSMovieEngine();

        bool playMovie(spark::MoviePtr theMovieWidget);
        void stopMovie(spark::MoviePtr theMovieWidget);
        void pauseMovie(spark::MoviePtr theMovieWidget);
        void resetMovie(spark::MoviePtr theMovieWidget);
        void updateMovieTexture(spark::MoviePtr theMovieWidget);
        const masl::VideoInfo getMovieInfo(spark::MoviePtr theMovieWidget) const;
        bool isMoviePlaying(spark::MoviePtr theMovieWidget) const;
        void setMovieVolume(spark::MoviePtr theMovieWidget, float newVolume) ;

    private:
        typedef std::map<spark::MoviePtr,MovieControllerPtr> MovieMap;
        MovieMap _movieMap;
        
    };
    typedef masl::Ptr<IOSMovieEngine> IOSMovieEnginePtr;
};

#endif
