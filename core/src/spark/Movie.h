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

#include "I18nShapeWidget.h"

namespace spark {
    class Movie : public I18nShapeWidget {
    public:
        Movie(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Movie();
        
        virtual void prerender(MatrixStack& theCurrentMatrixStack);
        inline const std::string & getSrc() const {return getData();};
        
        void play();   // start playback at playcursor
        void stop();   // stop videoplayback
        void pause();  // pause video and do not touch playcursor
        bool isPlaying();
        void togglePlayPause();
        void setVolume(const float theNewVolume);
        float getVolume();
        
        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Movie::SPARK_TYPE;};

    protected:
        virtual void build();
    private:
        float _volume;
    };

    typedef masl::Ptr<Movie> MoviePtr;
    typedef MemberFunctionEventCallback<Movie, MoviePtr> MovieCB;
};
#endif
