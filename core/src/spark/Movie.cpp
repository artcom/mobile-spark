// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "Movie.h"

#include <masl/MovieEngine.h>
#include <masl/MobileSDK.h>

using namespace mar;

namespace spark {
    const char * const Movie::SPARK_TYPE = "Movie";

    Movie::Movie(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        Image(theApp, theXMLNode),
        _volume(_myXMLNode->getAttributeAs<float>("volume", 1.f))
    {
#ifdef ANDROID
        _myFragmentShader = ANDROID_MOVIE_FRAGMENT_SHADER; 
#endif
    }

    Movie::~Movie() {
        AC_INFO << "....destructor Movie " << getName();
    }
    
    void
    Movie::prerender(MatrixStack& theCurrentMatrixStack) {
        ShapeWidget::prerender(theCurrentMatrixStack);
        
        if (isPlaying() && isRendered()) {
            // retrieve MovieInfo
            masl::VideoInfo myMovieInfo = masl::MovieEngineSingleton::get().getNative()->getMovieInfo(this);
            
            // load Material
            mar::UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<mar::UnlitTexturedMaterial>(getShape()->elementList_[0]->material_);
            
            // trigger update for OpenGL texture
            masl::MovieEngineSingleton::get().getNative()->updateMovieTexture(this);
            
            // inject texture name 
            myMaterial->getTextureUnit()->getTexture()->_textureId = myMovieInfo.textureID;
#ifdef ANDROID
            myMaterial->getTextureUnit()->getTexture()->_textureTarget = GL_TEXTURE_EXTERNAL_OES;
#endif
            
            _myTextureSize = vector2(myMovieInfo.width, myMovieInfo.height);
            _myRealImageSize = _myTextureSize;
            
            float myWidth = _myForcedSize[0] == -1 ? _myRealImageSize[0] : _myForcedSize[0];
            float myHeight = _myForcedSize[1] == -1 ? _myRealImageSize[1] : _myForcedSize[1];

            // adjust widget´s size
            I18nShapeWidget::setSize(vector2(myWidth, myHeight));
        } else {
            // TODO: stop movie if playing here !?
        }
    }
    
    void
    Movie::build() {
        I18nShapeWidget::build();
        if(getSrc().empty()) return;
        
        AC_DEBUG<<"build movie " << *this << " with src: "<<getSrc();
        UnlitTexturedMaterialPtr myMaterial;
        
        if (!getShape()) {
            myMaterial = UnlitTexturedMaterialPtr(new UnlitTexturedMaterial());
            myMaterial->setCustomHandles(_myCustomShaderValues);
            myMaterial->setShader(_myVertexShader, _myFragmentShader); 
            _myShape = createCustomShape(myMaterial);
        } else {
            myMaterial = boost::static_pointer_cast<UnlitTexturedMaterial>(getShape()->elementList_[0]->material_);
            myMaterial->getTextureUnit()->getTexture()->unbind();
        }
        float myWidth = _myForcedSize[0] == -1 ? 1 : _myForcedSize[0];
        float myHeight = _myForcedSize[1] == -1 ? 1 : _myForcedSize[1];

        // adjust widget´s size
        I18nShapeWidget::setSize(vector2(myWidth, myHeight));
    }

    void
    Movie::play() {
        masl::MovieEngineSingleton::get().getNative()->playMovie(this, getSrc());
        setVolume(_volume);
        //XXX: rethink
        // flip texcoords just now to have correct coords for movie-textures
        _myShape->setTexCoords(vector2(0, 1), vector2(1, 1),
                               vector2(0, 0), vector2(1, 0));
        
        AC_INFO<<"playing movie:"<< getSrc() << " (volume: "<<_volume<<")";
    }

    void
    Movie::stop() {
        masl::MovieEngineSingleton::get().getNative()->stopMovie(this);
    }

    void
    Movie::pause() {
        masl::MovieEngineSingleton::get().getNative()->pauseMovie(this);
    }

    bool
    Movie::isPlaying() {
        return  masl::MovieEngineSingleton::get().getNative()->isMoviePlaying(this);
    }
    
    void
    Movie::setVolume(float newVolume) {
        _volume = cml::clamp(newVolume, 0.f, 1.f);
        //if(isPlaying())
        {
            masl::MovieEngineSingleton::get().getNative()->setMovieVolume(this, _volume);
        }
    }
    
    float
    Movie::getVolume() {
        return _volume;
    }
    
    void
    Movie::togglePlayPause() {
        if(isPlaying()) {
            pause();
        } else {
            play();
        }
    }
}
