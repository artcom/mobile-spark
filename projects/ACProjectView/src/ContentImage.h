// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_acprojectview_ContentImage_
#define _included_mobile_acprojectview_ContentImage_

#include <spark/Container.h>

namespace acprojectview {
        


    class ContentImage : public spark::Container {
        public: 
            ContentImage(const spark::BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~ContentImage();
            
            static const char * const SPARK_TYPE;             
            virtual const char * const & getType() const { return ContentImage::SPARK_TYPE;};
            
            std::string getSrc();
            
        private:
            std::string _mySrc;
    };
    typedef masl::Ptr<ContentImage> ContentImagePtr;
};

#endif
