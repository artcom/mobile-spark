// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_templateapp_TemplateApp_
#define _included_mobile_templateapp_TemplateApp_

/////////////////// Application code, this should be in java or script language later...
#include <spark/BaseApp.h>

#ifdef ANDROID
extern "C" {
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved);
}
#endif

namespace templateapp {
    class TemplateApp: public spark::BaseApp {
        public: 
            TemplateApp();
            virtual ~TemplateApp();
            virtual void setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, int theScreenWidth, int theScreenHeight);

        private:
        	
    };

    typedef masl::Ptr<TemplateApp> TemplateAppPtr;
    typedef masl::WeakPtr<TemplateApp> TemplateAppWeakPtr;
    typedef spark::MemberFunctionEventCallback<TemplateApp, TemplateAppWeakPtr> TemplateAppEventCB;

};

#endif 

