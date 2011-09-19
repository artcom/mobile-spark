#ifndef _included_mobile_projectviewapp_ProjectViewApp_
#define _included_mobile_projectviewapp_ProjectViewApp_

/////////////////// Application code, this should be in java or script language later...
#include <spark/BaseApp.h>
#include "ProjectMenuItemImpl.h"



#ifdef __ANDROID__
extern "C" {
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved);
}
#endif

namespace projectviewapp {
    class ProjectViewApp: public spark::BaseApp {
        public: 
            ProjectViewApp();
            virtual ~ProjectViewApp();
            virtual void setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, int theScreenWidth, int theScreenHeight);

        private:
                void onProjectItem(spark::EventPtr theEvent);

        std::vector<ProjectMenuItemImplPtr> _myProjects;
        ProjectMenuItemImplPtr _myCurrentProject; 
        spark::ComponentPtr _myProjectMenu;
        spark::ContainerPtr _myProjectItems;      	
    };

    typedef boost::shared_ptr<ProjectViewApp> ProjectViewAppPtr;
    typedef spark::MemberFunctionEventCallback<ProjectViewApp, ProjectViewAppPtr> ProjectViewEventCB;

};

#endif 

