#ifndef _included_mobile_acprojectview_ACProjectView_
#define _included_mobile_acprojectview_ACProjectView_

/////////////////// Application code, this should be in java or script language later...
#include <masl/RequestManager.h>
#include <animation/SequenceAnimation.h>
#include <animation/DelayAnimation.h>
#include <animation/ParallelAnimation.h>
#include <spark/BaseApp.h>
#include <spark/Event.h>
#include "ProjectImpl.h"
#include "ProjectMenu.h"
#include "ProjectViewerImpl.h"
#include "ContentImage.h"

#include <boost/progress.hpp>

#ifdef ANDROID
extern "C" {
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved);
}
#endif

namespace acprojectview {
    class ACProjectView: public spark::BaseApp {
        public: 
            ACProjectView();
            virtual ~ACProjectView();
            virtual void setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, int theScreenWidth, int theScreenHeight);
            
        private:
            void onProjectItem(spark::EventPtr theEvent);
            void onLanguageSwitchDe(spark::EventPtr theEvent);
            void onLanguageSwitchEn(spark::EventPtr theEvent);
            void onWorldRealized(spark::EventPtr theEvent);
                
            void onBack(spark::EventPtr theEvent);
            void onStartProjectView();
            void onInitiateProjectView();
            void onFinishLoadProjectView();
            void onReturn2ProjectView();
            void projectViewAnimation(bool showProject);
            void onStartIdleFade();
            void onFinishIdleFade();            
            void closeProjectView();
            void onShowProjectViewPopup();
            void onLoadInitialSet0();
            void onLoadInitialSet1();
            void onLoadInitialSet2();
            void onLoadInitialSet3();
            void onLoadInitialSet4();
            
            //idle
            void initIdle();
            void updateKenBurnsShader(float theProgress);
            void onIdle();
            void onKenBurnsImageFadeStart();
            void onKenBurnsImageFadeEnd();
            void onTouch(spark::EventPtr theEvent);
            bool firstIdleImageVisible_;
            bool swappedIdleImages_;
            std::vector<std::string> idleFiles_;
            std::vector<spark::ImagePtr> _myIdleScreenImagePtrs;
            animation::DelayAnimationPtr _myIdleDelay;
            animation::ParallelAnimationPtr _myKenBurnsAnimation;
            const static unsigned int _myKenBurnsDuration;
            const static unsigned int _myKenBurnsFadeDuration;
            const static float d;
            const static float rd;
            const static float lt;
            const static float ut;

            ProjectImplPtr _myCurrentProject; 
            ProjectViewerImplPtr _myProjectViewer; 
            ProjectMenuPtr _myProjectMenu;
            spark::ContainerPtr _myProjectItems;  
            spark::TransformPtr _myStartScreenPtr;
        
            bool _myAnimatingFlag;
            animation::SequenceAnimationPtr _mySeqAnimation;
            const static unsigned int _myAnimationTime = 400;
            const static unsigned int _myIdleTime = 40000;
            
            void onFrame(spark::EventPtr theEvent);
            
            //HTTP-Requests
            masl::RequestManager _myRequestManager;
            int loadCount_;
            const static std::string BASE_URL;
            void onAssetReady(masl::RequestPtr theRequest);
            void onLoadComplete();
    };

    typedef masl::Ptr<ACProjectView> ACProjectViewPtr;
    typedef masl::WeakPtr<ACProjectView> ACProjectViewWeakPtr;
    typedef masl::MemberFunctionCallback<ACProjectView, ACProjectViewWeakPtr> ACProjectViewCB;
    typedef masl::MemberFunctionRequestCallback<ACProjectView, ACProjectViewWeakPtr> ACProjectViewRequestCB;
    typedef spark::MemberFunctionEventCallback<ACProjectView, ACProjectViewWeakPtr> ACProjectViewEventCB;

    //animations
    typedef void (ACProjectView::* ACProjectViewPropertySetterFunction)(float);
    typedef void (ACProjectView::* ACProjectViewMemberFunction)();
    typedef animation::PropertyAnimation<ACProjectViewWeakPtr, ACProjectViewPropertySetterFunction> ACProjectViewPropertyAnimation;
    typedef masl::Ptr<ACProjectViewPropertyAnimation>  ACProjectViewPropertyAnimationPtr;

};

#endif 

