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
            virtual void setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, int theScreenWidth, int theScreenHeight);

        private:
        	void onSwipeGesture(spark::EventPtr theEvent);
        	void onPinchGesture(spark::EventPtr theEvent);
        	void onPanGesture(spark::EventPtr theEvent);
        	void onSensorEvent(spark::EventPtr theEvent);
        	void onSensorLightEvent(spark::EventPtr theEvent);
        	void onSensorGyroEvent(spark::EventPtr theEvent);
            void onControlButton(spark::EventPtr theEvent);
            void onCreationButton(spark::EventPtr theEvent);
            void onLanguageSwitch(spark::EventPtr theEvent);
            void onTouch(spark::EventPtr theEvent);
            void insertCreatedComponent();
            void onSizeChanged(spark::EventPtr theEvent);
            void onFrame(spark::EventPtr theEvent);


            void centerSlideTitlesToNewCanvasSize(int theWidth, int theHeight);
            void onStartSlideSwipe();
            void onFinishSlideSwipe();
            void changeSlide(int theDirection);
            
            std::vector<SlideImplPtr> _mySlides;
            unsigned _myCurrentSlide;
            unsigned _myNextSlide;
    };

    typedef masl::Ptr<DemoApp> DemoAppPtr;
    typedef spark::MemberFunctionEventCallback<DemoApp, DemoAppPtr> DemoEventCB;

};

#endif 

