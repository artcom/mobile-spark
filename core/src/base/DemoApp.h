#ifndef _included_mobile_asl_DemoApp_
#define _included_mobile_asl_DemoApp_

/////////////////// Application code, this should be in java or script language later...
#include "BaseApp.h"

namespace asl {
    class DemoApp: public BaseApp {
        public: 
            DemoApp();
            virtual ~DemoApp();
    };

    typedef boost::shared_ptr<DemoApp> DemoAppPtr;

};

#endif 

