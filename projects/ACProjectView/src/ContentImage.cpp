// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "ContentImage.h"
#include <spark/SparkComponentFactory.h>

using namespace spark;

namespace acprojectview {

    const char * const ContentImage::SPARK_TYPE = "ContentImage";
    
        
    ContentImage::ContentImage(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
        : Container(theApp, theXMLNode) {
        _mySrc = _myXMLNode->getAttributeAs<std::string>("src",""); 
    }

    ContentImage::~ContentImage() {}

    std::string ContentImage::getSrc() {return _mySrc;}
}
