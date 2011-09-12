#ifndef _included_mobile_demoapp_DemoApp_
#define _included_mobile_demoapp_DemoApp_

/////////////////// Application code, this should be in java or script language later...
#include <spark/BaseApp.h>
#include <spark/Transform.h>
#include <spark/View.h>
#include "Slide.h"

#ifdef __ANDROID__
extern "C" {
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved);
}
#endif

namespace demoapp {
    class DemoApp: public spark::BaseApp {
        public: 
            DemoApp();
            virtual ~DemoApp();
            virtual void setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath);

        private:
            void onControlButton(spark::EventPtr theEvent);
            void onCreationButton(spark::EventPtr theEvent);
            void onTouch(spark::EventPtr theEvent);
            void insertCreatedComponent();
            void onSizeChanged(int theWidth, int theHeight);


            void centerSlideTitlesToNewCanvasSize(int theWidth, int theHeight);

            std::vector<SlideImplPtr> _mySlides;
            std::vector<spark::ViewPtr> _myViews;
            unsigned _myCurrentSlide;
    };

    typedef boost::shared_ptr<DemoApp> DemoAppPtr;
    typedef spark::MemberFunctionEventCallback<DemoApp, DemoAppPtr> DemoEventCB;

};

#endif 

