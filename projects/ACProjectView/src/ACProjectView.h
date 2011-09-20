#ifndef _included_mobile_acprojectview_ACProjectView_
#define _included_mobile_acprojectview_ACProjectView_

/////////////////// Application code, this should be in java or script language later...
#include <spark/BaseApp.h>
#include "ProjectMenuItemImpl.h"

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

            std::vector<ProjectMenuItemImplPtr> _myProjects;
            ProjectMenuItemImplPtr _myCurrentProject; 
            spark::ComponentPtr _myProjectMenu;
            spark::ContainerPtr _myProjectItems;      	
    };

    typedef boost::shared_ptr<ACProjectView> ACProjectViewPtr;
    typedef spark::MemberFunctionEventCallback<ACProjectView, ACProjectViewPtr> ACProjectViewEventCB;

};

#endif 

