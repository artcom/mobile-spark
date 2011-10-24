// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_spark_AppProvider_h_included_
#define _ac_mobile_spark_AppProvider_h_included_

#include <masl/Singleton.h>
#include <masl/Logger.h>
#include "BaseApp.h"

namespace spark {
    class AppProvider : public masl::Singleton<AppProvider> {
    public:
        AppProvider() { };
        virtual ~AppProvider();
        void setApp(BaseAppPtr theApp);
        BaseAppPtr getApp() const { return app_; };
    private:
        BaseAppPtr app_;
    };
};

#endif

