// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "Movie.h"

#include <masl/MobileSDK.h>

using namespace mar;

namespace spark {
    const char * const Movie::SPARK_TYPE = "Movie";

    Movie::Movie(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        Image(theApp, theXMLNode)
    {
        _moviesrc = _myXMLNode->getAttributeAs<std::string>("moviesrc", "");
        
        _volume = _myXMLNode->getAttributeAs<float>("volume", 1.f);
        
    }

    Movie::~Movie() {
        AC_INFO << "....destructor Movie " << getName();
    }
    
    void Movie::prerender(MatrixStack& theCurrentMatrixStack)
    {
        ShapeWidget::prerender(theCurrentMatrixStack);
        
        if (isPlaying() && isRendered()) 
        {
            // Construct a shared ptr to this
            MoviePtr thisPtr = boost::static_pointer_cast<Movie>(shared_from_this());
            
            // retrieve MovieInfo
            masl::VideoInfo myMovieInfo = masl::MobileSDK_Singleton::get().getNative()->getMovieInfo(thisPtr);
            
            // load Material
            mar::UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<mar::UnlitTexturedMaterial>(getShape()->elementList_[0]->material_);
            
            // trigger update for OpenGL texture
            masl::MobileSDK_Singleton::get().getNative()->updateMovieTexture(thisPtr);
            
            // inject texture name 
            myMaterial->getTextureUnit()->getTexture()->_textureId = myMovieInfo.textureID;
            
            _myTextureSize = vector2(myMovieInfo.width, myMovieInfo.height);
            _myRealImageSize = _myTextureSize;
            
            float myWidth = _myForcedSize[0] == -1 ? _myRealImageSize[0] : _myForcedSize[0];
            float myHeight = _myForcedSize[1] == -1 ? _myRealImageSize[1] : _myForcedSize[1];

            // adjust widget´s size
            I18nShapeWidget::setSize(vector2(myWidth, myHeight));
        } 
        else 
        {
            // TODO: stop movie if playing here !?
        }
    }
    
    void Movie::play() 
    {
        MoviePtr thisPtr = boost::static_pointer_cast<Movie>(shared_from_this());
        
        masl::MobileSDK_Singleton::get().getNative()->playMovie(thisPtr);
        setVolume(_volume);
        
        mar::UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<mar::UnlitTexturedMaterial>(getShape()->elementList_[0]->material_);
        myMaterial->getTextureUnit()->getTexture()->_textureId = 0;
        
        // flip texcoords just now to have correct coords for movie-textures
        _myShape->setTexCoords(vector2(0, 1), vector2(1, 1),
                               vector2(0, 0), vector2(1, 0));
        
        AC_PRINT<<"playing movie:"<< _moviesrc<<" (volume: "<<_volume<<")";
        
    }
    void Movie::stop()
    {
        MoviePtr thisPtr = boost::static_pointer_cast<Movie>(shared_from_this());
        
        masl::MobileSDK_Singleton::get().getNative()->stopMovie(thisPtr);
    }
    void Movie::pause()
    {
        MoviePtr thisPtr = boost::static_pointer_cast<Movie>(shared_from_this());
        
        masl::MobileSDK_Singleton::get().getNative()->pauseMovie(thisPtr);
    }
    void Movie::reset()
    {
        MoviePtr thisPtr = boost::static_pointer_cast<Movie>(shared_from_this());
        
        masl::MobileSDK_Singleton::get().getNative()->resetMovie(thisPtr);
    }
    
    bool Movie::isPlaying()
    {
        MoviePtr thisPtr = boost::static_pointer_cast<Movie>(shared_from_this());
        return  masl::MobileSDK_Singleton::get().getNative()->isMoviePlaying(thisPtr);
    }
    
    void Movie::setVolume(float newVolume)
    {
        float val = newVolume;
        if(val < 0.f) val = 0.f;
        if(val > 1.f) val = 1.f;
        
        _volume = val;
        
        //if(isPlaying())
        {
            MoviePtr thisPtr = boost::static_pointer_cast<Movie>(shared_from_this());
            masl::MobileSDK_Singleton::get().getNative()->setMovieVolume(thisPtr, _volume);
        }
    }
    
    float Movie::getVolume()
    {
        return _volume;
    }
    
    void Movie::togglePlayPause()
    {
        if(isPlaying())
            pause();
        else
            play();

    }
}
