// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_demoapp_DemoApp_
#define _included_mobile_demoapp_DemoApp_

/////////////////// Application code, this should be in java or script language later...
#include <masl/RequestManager.h>
#include <animation/Animation.h>
#include <spark/BaseApp.h>
#include <spark/Transform.h>
#include <spark/View.h>
#include "Slide.h"

#ifdef ANDROID
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
            virtual void onPause();
            virtual void onResume();

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
            void onLoadScene(spark::EventPtr theEvent);
            void onPlaySound1(spark::EventPtr theEvent);
            void onPlaySound2(spark::EventPtr theEvent);
            void onPlaySound3(spark::EventPtr theEvent);

            void centerSlideTitlesToNewCanvasSize(int theWidth, int theHeight);
            void onStartSlideSwipe();
            void onFinishSlideSwipe();
            void changeSlide(int theDirection);
            
            std::vector<SlideImplPtr> _mySlides;
            unsigned _myCurrentSlide;
            unsigned _myNextSlide;

            //http
            static const std::string BASE_URL;
            masl::RequestManager _myRequestManager;
            animation::AnimationPtr _myLoadingAnimation;
            animation::AnimationPtr _myErrorExpiredDelay;
            spark::TextPtr _myLoadingMessage;
            spark::TextPtr _myErrorMessage;
            void onRepeatingDateRequest();
            void onErrorExpired();
            void onTextRequestReady(masl::RequestPtr theRequest);
            void onDateRequestReady(masl::RequestPtr theRequest);
            void onSparkRequestReady(masl::RequestPtr theRequest);
            void onAssetRequestReady(masl::RequestPtr theRequest);
            void onAllAssetsRequestReady(masl::RequestPtr theRequest);
            void onGetRequestReady(masl::RequestPtr theRequest);
            void onHeadRequestReady(masl::RequestPtr theRequest);
            void onPostRequestReady(masl::RequestPtr theRequest);
            void onPutRequestReady(masl::RequestPtr theRequest);
            void onDeleteRequestReady(masl::RequestPtr theRequest);
            void onErrorRequestCB(masl::RequestPtr theRequest);
    };

    typedef masl::Ptr<DemoApp> DemoAppPtr;
    typedef masl::WeakPtr<DemoApp> DemoAppWeakPtr;
    typedef masl::MemberFunctionCallback<DemoApp, DemoAppWeakPtr> DemoCB;
    typedef spark::MemberFunctionEventCallback<DemoApp, DemoAppWeakPtr> DemoEventCB;
    typedef masl::MemberFunctionRequestCallback<DemoApp, DemoAppWeakPtr> DemoRequestCB;
};

#endif 

