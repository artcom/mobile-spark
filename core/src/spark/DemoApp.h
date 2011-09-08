#ifndef _included_mobile_spark_DemoApp_
#define _included_mobile_spark_DemoApp_

/////////////////// Application code, this should be in java or script language later...
#include "BaseApp.h"
#include "Transform.h"
#include "View.h"


namespace spark {
    class DemoApp: public BaseApp {
        public: 
            DemoApp();
            virtual ~DemoApp();
            virtual bool setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, const std::string & theLayoutFile);
            void onTouch(EventPtr theEvent);
            std::vector<TransformPtr> _mySlides;
            std::vector<ViewPtr> _myViews;
            unsigned _myCurrentSlide;
    };

    typedef boost::shared_ptr<DemoApp> DemoAppPtr;

};

#endif 

