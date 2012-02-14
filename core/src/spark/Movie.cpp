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
        ShapeWidget(theApp, theXMLNode)
    {
        _moviesrc = _myXMLNode->getAttributeAs<std::string>("moviesrc", "");
        
        mar::MaterialPtr myMaterial = mar::MaterialPtr(new mar::UnlitTexturedMaterial());
        _myShape = mar::ShapePtr(new mar::RectangleShape(myMaterial));
    }

    Movie::~Movie() {
        AC_INFO << "....destructor Movie " << getName();
    }
    
    void Movie::prerender(MatrixStack& theCurrentMatrixStack)
    {
        ShapeWidget::prerender(theCurrentMatrixStack);
        
        if (isRendered()) 
        {
            // Construct a shared ptr to this
            MoviePtr thisPtr = boost::static_pointer_cast<Movie>(shared_from_this());
            
            // retrieve MovieInfo
            masl::MovieInfo myMovieInfo = masl::MobileSDK_Singleton::get().getNative()->getMovieInfo(thisPtr);
            
            // load Material
            mar::UnlitTexturedMaterialPtr myMaterial = boost::static_pointer_cast<mar::UnlitTexturedMaterial>(getShape()->elementList_[0]->material_);
            
            // trigger update for OpenGL texture
            masl::MobileSDK_Singleton::get().getNative()->updateMovieTexture(thisPtr);
            
            // inject texture name 
            myMaterial->getTextureUnit()->getTexture()->textureId_ = myMovieInfo.textureID;
            
        } 
        else 
        {
            // stop movie if playing
        }
    }
    
    void Movie::play() 
    {
        MoviePtr thisPtr = boost::static_pointer_cast<Movie>(shared_from_this());
        
        masl::MobileSDK_Singleton::get().getNative()->playMovie(thisPtr);
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
}
