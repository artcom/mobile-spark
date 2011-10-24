// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_demoapp_Slide_
#define _included_mobile_demoapp_Slide_

#include <spark/Transform.h>
#include <spark/Text.h>

namespace demoapp {

    class SlideImpl : public spark::Transform {
        public: 
            SlideImpl(const spark::BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~SlideImpl();
            void centerTitles(const int theWidth, const int theHeight);
            static const char * const SPARK_TYPE;             
            virtual const char * const & getType() const { return SlideImpl::SPARK_TYPE;};
        private:
            spark::TextPtr titleComponent_;
            spark::TextPtr subtitleComponent_;
    };
    typedef masl::Ptr<SlideImpl> SlideImplPtr;
    typedef masl::WeakPtr<SlideImpl> SlideImplWeakPtr;
};

#endif
