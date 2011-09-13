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

