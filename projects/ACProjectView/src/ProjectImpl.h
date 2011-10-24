// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_acprojectview_ProjectImpl_
#define _included_mobile_acprojectview_ProjectImpl_

#include <spark/Transform.h>
#include <spark/Image.h>
#include <spark/Text.h>


namespace acprojectview {
    
    class ProjectImpl : public spark::Transform {
        public: 
            ProjectImpl(const spark::BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~ProjectImpl();
            virtual void realize();
            virtual void onResume();
            void fitToSize();
            
            static const char * const SPARK_TYPE;             
            virtual const char * const & getType() const { return ProjectImpl::SPARK_TYPE;};
        private:
            spark::ImagePtr imageComponent_; 
            spark::TextPtr titleComponent_;
            spark::TextPtr subtitleComponent_;
    };
    
    typedef masl::Ptr<ProjectImpl> ProjectImplPtr;
};

#endif
