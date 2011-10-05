#ifndef _included_mobile_acprojectview_ACProjectView_
#define _included_mobile_acprojectview_ACProjectView_

/////////////////// Application code, this should be in java or script language later...
#include <animation/DelayAnimation.h>
#include <animation/ParallelAnimation.h>
#include <spark/BaseApp.h>
#include <spark/Event.h>
#include "ProjectImpl.h"
#include "ProjectMenu.h"
#include "ProjectViewerImpl.h"
#include "ContentImage.h"


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
            void onSwipeCB(spark::EventPtr theEvent);
            void onStartScreenClicked(spark::EventPtr theEvent);
            void onStartProjectView();
            void onInitiateProjectView();
            void onFinishLoadProjectView();
            void onReturn2ProjectView();
            void projectViewAnimation(bool showProject);
            void onStartIdleFade();
            void onFinishIdleFade();            
            void initIdle();
            void onIdle();
            void onTouch(spark::EventPtr theEvent);
            void onFinishProjectView();

            
            ProjectImplPtr _myCurrentProject; 
            ProjectViewerImplPtr _myProjectViewer; 
            ProjectMenuPtr _myProjectMenu;
            spark::ContainerPtr _myProjectItems;  
            spark::TransformPtr _myStartScreenPtr;

            spark::ImagePtr _myIdleScreenImagePtr;
            animation::DelayAnimationPtr _myIdleDelay;
            animation::ParallelAnimationPtr _myKenBurnsAnimation;
        
            unsigned int _myWidth;
            unsigned int _myHeight;

            const static unsigned int _myAnimationTime = 400;
            const static unsigned int _myIdleTime = 50000;
    };

    typedef masl::Ptr<ACProjectView> ACProjectViewPtr;
    typedef spark::MemberFunctionEventCallback<ACProjectView, ACProjectViewPtr> ACProjectViewEventCB;
};

#endif 

