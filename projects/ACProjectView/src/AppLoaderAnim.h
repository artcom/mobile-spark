// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_acprojectview_AppLoaderAnim_
#define _included_mobile_acprojectview_AppLoaderAnim_

#include <spark/Transform.h>
#include <spark/Event.h>
#include <spark/Image.h>

namespace acprojectview {

    class AppLoaderAnim : public spark::Transform {
        public: 
            AppLoaderAnim(const spark::BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~AppLoaderAnim();
            
            static const char * const SPARK_TYPE;             
            virtual const char * const & getType() const { return AppLoaderAnim::SPARK_TYPE;};
                
            void removeOnFrameListener();
            void realize();
            void onFrame(spark::EventPtr theEvent);

                    
        private:
            double _myLastTime;
            int _myCurrentImageIndex;
            std::vector<spark::ImagePtr> _myImages;
            spark::EventCallbackPtr _myFrameCB;
            
    };
    typedef masl::Ptr<AppLoaderAnim> AppLoaderAnimPtr;
    typedef masl::WeakPtr<AppLoaderAnim> AppLoaderAnimWeakPtr;
    typedef spark::MemberFunctionEventCallback<AppLoaderAnim, AppLoaderAnimWeakPtr> AppLoaderAnimCB;
        
};

#endif
