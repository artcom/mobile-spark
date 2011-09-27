#ifndef _included_mobile_acprojectview_ACProjectView_
#define _included_mobile_acprojectview_ACProjectView_

/////////////////// Application code, this should be in java or script language later...
#include <spark/BaseApp.h>
#include "ProjectImpl.h"
#include "ProjectMenu.h"
#include "ProjectViewerImpl.h"
#include "ContentImage.h"
#include "Description.h"


#ifdef __ANDROID__
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
            void onLanguageSwitch(spark::EventPtr theEvent);
            void onBack(spark::EventPtr theEvent);
            void onSwipeCB(spark::EventPtr theEvent);
            void onStartScreenClicked(spark::EventPtr theEvent);
            void onStartProjectView();
            void onInitiateProjectView();
            void onFinishLoadProjectView();
            void projectViewAnimation(bool showProject);
            void onStartIdleFade();
            void onFinishIdleFade();            
            ProjectImplPtr _myCurrentProject; 
            ProjectViewerImplPtr _myProjectViewer; 
            ProjectMenuPtr _myProjectMenu;
            spark::ContainerPtr _myProjectItems;  
            spark::TransformPtr _myStartScreenPtr;
        
            unsigned int _myWidth;
            unsigned int _myHeight;

            const static unsigned int _myAnimationTime = 400;
    };

    typedef boost::shared_ptr<ACProjectView> ACProjectViewPtr;
    typedef spark::MemberFunctionEventCallback<ACProjectView, ACProjectViewPtr> ACProjectViewEventCB;
};

#endif 

