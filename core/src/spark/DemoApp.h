#ifndef _included_mobile_spark_DemoApp_
#define _included_mobile_spark_DemoApp_

/////////////////// Application code, this should be in java or script language later...
#include "BaseApp.h"

namespace spark {
    class DemoApp: public BaseApp {
        public: 
            DemoApp();
            virtual ~DemoApp();
            virtual bool setup(const long theCurrentMillis, const std::string & theAssetPath, const std::string & theLayoutFile);
            virtual void onTouch();
    };

    typedef boost::shared_ptr<DemoApp> DemoAppPtr;

};

#endif 

