#ifndef _included_mobile_demoapp_DemoApp_
#define _included_mobile_demoapp_DemoApp_

/////////////////// Application code, this should be in java or script language later...
#include <spark/BaseApp.h>
#include <spark/Transform.h>
#include <spark/View.h>


namespace demoapp {
    class DemoApp: public BaseApp {
        public: 
            DemoApp();
            virtual ~DemoApp();
            virtual bool setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, const std::string & theLayoutFile);

        private:
            void onControlButton(EventPtr theEvent);
            void onCreationButton(EventPtr theEvent);
            void onTouch(EventPtr theEvent);
            void insertCreatedComponent();

            std::vector<TransformPtr> _mySlides;
            std::vector<ViewPtr> _myViews;
            unsigned _myCurrentSlide;
    };

    typedef boost::shared_ptr<DemoApp> DemoAppPtr;
    typedef MemberFunctionEventCallback<DemoApp, DemoAppPtr> DemoEventCB;

};

#endif 

