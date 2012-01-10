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
    }

    Movie::~Movie() {
        AC_INFO << "....destructor Movie " << getName();
    }
    
    void Movie::play() {
         masl::MobileSDK_Singleton::get().getNative()->playMovie(moviesrc_);
    }
    void Movie::stop(){
         masl::MobileSDK_Singleton::get().getNative()->stopMovie();
    }
    void Movie::pause(){
         masl::MobileSDK_Singleton::get().getNative()->pauseMovie();
    }
    void Movie::reset(){
         masl::MobileSDK_Singleton::get().getNative()->resetMovie();
    }    
}
