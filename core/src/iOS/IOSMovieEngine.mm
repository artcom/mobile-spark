// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "IOSMovieEngine.h"
#include "MovieController.h"

namespace ios
{
    void IOSMovieEngine::playMovie(void* theMovieWidget, const std::string & theSrc) 
    {
        std::string filePath;
        
        if (theSrc.empty() || !masl::searchFile(theSrc, filePath)) { 
             throw masl::Exception("Movie " + theSrc + " not found", PLUS_FILE_LINE);
        }
        
        MovieMap::const_iterator it = _movieMap.find(theMovieWidget);
        if(it == _movieMap.end())
            _movieMap[theMovieWidget] = MovieControllerPtr(new MovieController);
        
        _movieMap[theMovieWidget]->playMovie(filePath);
    }
    
    void IOSMovieEngine::stopMovie(void* theMovieWidget) 
    {
        _movieMap[theMovieWidget]->stop();
        
        _movieMap.erase(theMovieWidget);
    }
    
    void IOSMovieEngine::pauseMovie(void* theMovieWidget) 
    {
        _movieMap[theMovieWidget]->pause();
    }
    
    void IOSMovieEngine::resetMovie(void* theMovieWidget) 
    {
        _movieMap[theMovieWidget]->reset();
    }
    
    void IOSMovieEngine::updateMovieTexture(void* theMovieWidget)
    {
        MovieMap::iterator it = _movieMap.find(theMovieWidget);
        
        if(it != _movieMap.end())
            it->second->copyNextFrameToTexture();
    }
    
    const masl::VideoInfo IOSMovieEngine::getMovieInfo(void* theMovieWidget) const
    {
        masl::VideoInfo movieInfo;
        
        MovieMap::const_iterator it = _movieMap.find(theMovieWidget);
        
        if(it != _movieMap.end())
        {
            movieInfo.textureID = it->second->getTextureID();
            movieInfo.width = it->second->getWidth();
            movieInfo.height = it->second->getHeight();
        }
        
        return movieInfo;
    }
    
    bool IOSMovieEngine::isMoviePlaying(void* theMovieWidget) const
    {
        bool ret = false;
        
        MovieMap::const_iterator it = _movieMap.find(theMovieWidget);
        
        if(it != _movieMap.end())
        {
            ret = it->second->isPlaying();
        }

        return ret;
    }
}
