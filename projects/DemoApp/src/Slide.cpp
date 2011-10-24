// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "Slide.h"
#include <spark/SparkComponentFactory.h>

using namespace spark;

namespace demoapp {
    const char * const SlideImpl::SPARK_TYPE = "SlideImpl";             
    
    SlideImpl::SlideImpl(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
        : Transform(theApp, theXMLNode) {
        std::string title = _myXMLNode->getAttributeAs<std::string>("title",""); 
        std::string subtitle = _myXMLNode->getAttributeAs<std::string>("subtitle",""); 
        titleComponent_ = boost::static_pointer_cast<Text>(getChildByName("title"));
        subtitleComponent_ = boost::static_pointer_cast<Text>(getChildByName("subtitle"));
        if (title.size() > 0) {
            titleComponent_->setText(title);
        }
        if (subtitle.size() > 0) {
            subtitleComponent_->setText(subtitle);
        }
    }

    SlideImpl::~SlideImpl() {}


    void
    SlideImpl::centerTitles(const int theWidth, const int theHeight) {
        vector2 myTextSize = titleComponent_->getTextSize();
        titleComponent_->setX(theWidth/2.0 - myTextSize[0] / 2.0);
        titleComponent_->setY(theHeight - myTextSize[1] - 100);
        myTextSize = subtitleComponent_->getTextSize();
        subtitleComponent_->setX(theWidth/2.0 - myTextSize[0] / 2.0);
        subtitleComponent_->setY(titleComponent_->getY() - myTextSize[1] - 30);
    }
}
