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
        moviesrc_ = _myXMLNode->getAttributeAs<std::string>("moviesrc", "");
        
        //play();
    }

    Movie::~Movie() {
        AC_INFO << "....destructor Movie " << getName();
    }
    
    void Movie::prerender(MatrixStack& theCurrentMatrixStack)
    {
        ShapeWidget::prerender(theCurrentMatrixStack);
        
        if (isRendered()) 
        {
            // update texture here 
            
            // ( callback for new movie frames would be a better place to put this )

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
