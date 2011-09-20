#ifndef _included_mobile_acprojectview_ACProjectView_
#define _included_mobile_acprojectview_ACProjectView_

/////////////////// Application code, this should be in java or script language later...
#include <spark/BaseApp.h>
#include "ProjectImpl.h"
#include "ProjectMenu.h"
#include "ProjectViewerImpl.h"
#include "ContentImage.h"


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
            void onBack(spark::EventPtr theEvent);
            void projectViewAnimation(int fromX, int toX, int fromY, int toY, int fromScale, int toScale);
            

            //std::vector<ProjectImplPtr> _myProjects;
            ProjectImplPtr _myCurrentProject; 
            ProjectViewerImplPtr _myProjectViewer; 
            ProjectMenuPtr _myProjectMenu;
            spark::ContainerPtr _myProjectItems;  
        
            const static unsigned int _myAnimationTime = 250;
    };

    typedef boost::shared_ptr<ACProjectView> ACProjectViewPtr;
    typedef spark::MemberFunctionEventCallback<ACProjectView, ACProjectViewPtr> ACProjectViewEventCB;
};

#endif 

