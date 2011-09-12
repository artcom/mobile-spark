#ifndef _included_mobile_spark_DemoApp_
#define _included_mobile_spark_DemoApp_

/////////////////// Application code, this should be in java or script language later...
#include "BaseApp.h"
#include "View.h"
#include "Slide.h"


namespace spark {
    class DemoApp: public BaseApp {
        public: 
            DemoApp();
            virtual ~DemoApp();
            virtual bool setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, const std::string & theLayoutFile);

        private:
        	void onSwipeGesture(EventPtr theEvent);
            void onControlButton(EventPtr theEvent);
            void onCreationButton(EventPtr theEvent);
            void onTouch(EventPtr theEvent);
            void insertCreatedComponent();
            void onSizeChanged(int theWidth, int theHeight);


            void centerSlideTitlesToNewCanvasSize(int theWidth, int theHeight);

            std::vector<SlideImplPtr> _mySlides;
            std::vector<ViewPtr> _myViews;
            unsigned _myCurrentSlide;
    };

    typedef boost::shared_ptr<DemoApp> DemoAppPtr;
    typedef MemberFunctionEventCallback<DemoApp, DemoAppPtr> DemoEventCB;

};

#endif 

