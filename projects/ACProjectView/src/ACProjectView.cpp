#include "ACProjectView.h"
#include <cstdlib>

#include <boost/smart_ptr/shared_ptr.hpp>

#include <masl/Logger.h>
#include <masl/MobileSDK.h>

#include <spark/Event.h>
#include <spark/SparkComponentFactory.h>
#include <spark/AppProvider.h>

#include "ACProjectViewComponentMapInitializer.h"

using namespace spark;
using namespace masl;

/////////////////////////////////////////////////////////////////////////App-Instance
#ifdef __ANDROID__
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    spark::AppProvider::get().setApp(boost::shared_ptr<acprojectview::ACProjectView>(new acprojectview::ACProjectView()));
    return JNI_VERSION_1_6;
}
#endif


/////////////////// Application code, this should be in java or script language later...
namespace acprojectview {
   
    ACProjectView::ACProjectView():BaseApp("acprojectview") {
    }

    ACProjectView::~ACProjectView() {
    }

    void ACProjectView::setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, int theScreenWidth, int theScreenHeight) {
        BaseApp::setup(theCurrentMillis, theAssetPath, theScreenWidth, theScreenHeight);
        ACProjectViewComponentMapInitializer::init();
        std::string myOrientation;
        std::string mySparkFile = findBestMatchedLayout("/main", theScreenWidth, theScreenHeight, myOrientation);
        MobileSDK_Singleton::get().getNative()->freezeMobileOrientation(myOrientation);
        loadLayoutAndRegisterEvents(mySparkFile);
        BaseApp::realize();
        
        ACProjectViewPtr ptr = boost::static_pointer_cast<ACProjectView>(shared_from_this());
        _myProjectMenu = _mySparkWindow->getChildByName("2dworld")->getChildByName("main",true);

        
        spark::EventCallbackPtr myPickedCB = EventCallbackPtr(new ACProjectViewEventCB(ptr, &ACProjectView::onProjectItem));
      
        _myProjectItems = boost::static_pointer_cast<spark::Container>(_myProjectMenu);
        const VectorOfComponentPtr & myChildren = _myProjectItems->getChildrenByType("ProjectMenuItemImpl");
        for (size_t i = 0; i < myChildren.size(); i++) {
            ProjectMenuItemImplPtr myProject = boost::static_pointer_cast<ProjectMenuItemImpl>(myChildren[i]);
            if (myProject) {
                myProject->addEventListener(TouchEvent::PICKED, myPickedCB,true);
                _myProjects.push_back(myProject);
            }
        }
        
    }
    
    void ACProjectView::onProjectItem(EventPtr theEvent) {
        AC_PRINT << ":) __________ITEM: " << theEvent->getTarget()->getParent()->getName();
        boost::static_pointer_cast<Widget>(_myProjectItems)->setVisible(false);
    }

}


