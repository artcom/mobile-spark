// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_BaseApp_
#define _included_mobile_spark_BaseApp_

#ifdef ANDROID
#include "JNIBinding.h"
#endif

#include <masl/Settings.h>
#include <masl/ThreadLock.h>

#include "Event.h"

namespace spark {

    class Window;
    typedef masl::Ptr<Window> WindowPtr;

    class BaseApp : public boost::enable_shared_from_this<BaseApp> {
        public:
            BaseApp(const std::string & theAppPath);
            virtual ~BaseApp();

            virtual void setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, int theScreenWidth, int theScreenHeight);
            bool isSetup() const {return _mySetupFlag;}
            void loadLayoutAndRegisterEvents(int theScreenWidth, int theScreenHeight, const std::string & theBaseName = "main");
            virtual void onFrame(EventPtr theEvent);
            virtual void onPause();
            virtual void onResume();
            virtual void onEvent(const std::string & theEventString);
            virtual void handleEvents();
            virtual void onBackButton(EventPtr theEvent);
            
            void exit();
            spark::WindowPtr _mySparkWindow;
            
        protected:
            std::string appPath_;
            bool _mySetupFlag;       
        private:
            EventPtrList _myEvents;    
            masl::ThreadLock _myLock;

            
    };
    std::string findBestMatchedLayout(const std::string & theBaseName, int theScreenWidth, int theScreenHeight, bool &isPortrait);
    void assetProviderSetup(const std::string & theAssetPath, const std::string & theAppPath );

    typedef masl::Ptr<BaseApp> BaseAppPtr;
    typedef masl::WeakPtr<BaseApp> BaseAppWeakPtr;
};

#endif
