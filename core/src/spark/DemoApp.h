#ifndef _included_mobile_spark_DemoApp_
#define _included_mobile_spark_DemoApp_

/////////////////// Application code, this should be in java or script language later...
#include "BaseApp.h"

namespace spark {
    class DemoApp: public BaseApp {
        public: 
            DemoApp();
#ifdef __ANDROID__            
            bool setup(std::string apkPath, std::string layoutFile, JNIEnv* env);            
#endif                
            virtual ~DemoApp();
    };

    typedef boost::shared_ptr<DemoApp> DemoAppPtr;

};

#endif 

