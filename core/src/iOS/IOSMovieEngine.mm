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


#include "IOSMovieEngine.h"
//#include <masl/AssetProvider.h>
//#include <masl/CallStack.h>

namespace ios
{
    bool IOSMovieEngine::playMovie(spark::MoviePtr theMovieWidget) 
    {
        std::string filePath;
        
        if (theMovieWidget->getSrc().size() > 0 ) {
            if (!masl::searchFile(theMovieWidget->getSrc(), filePath)) 
                return false;
        }
        
        MovieMap::const_iterator it = _movieMap.find(theMovieWidget);
        if(it == _movieMap.end())
            _movieMap[theMovieWidget] = MovieControllerPtr(new MovieController);
        
        _movieMap[theMovieWidget]->playMovie(filePath);
        
        return true;
    }
    
    void IOSMovieEngine::stopMovie(spark::MoviePtr theMovieWidget) 
    {
        _movieMap[theMovieWidget]->stop();
        
        _movieMap.erase(theMovieWidget);
    }
    
    void IOSMovieEngine::pauseMovie(spark::MoviePtr theMovieWidget) 
    {
        _movieMap[theMovieWidget]->pause();
    }
    
    void IOSMovieEngine::resetMovie(spark::MoviePtr theMovieWidget) 
    {
        _movieMap[theMovieWidget]->reset();
    }
    
    void IOSMovieEngine::updateMovieTexture(spark::MoviePtr theMovieWidget)
    {
        MovieMap::iterator it = _movieMap.find(theMovieWidget);
        
        if(it != _movieMap.end())
            it->second->copyNextFrameToTexture();
    }
    
    const masl::VideoInfo IOSMovieEngine::getMovieInfo(spark::MoviePtr theMovieWidget) const
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
    
    bool IOSMovieEngine::isMoviePlaying(spark::MoviePtr theMovieWidget) const
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
