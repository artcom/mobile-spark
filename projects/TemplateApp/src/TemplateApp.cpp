#include "TemplateApp.h"
#include <cstdlib>

#include <masl/Logger.h>
#include <masl/MobileSDK.h>

#include <spark/Event.h>
#include <spark/SparkComponentFactory.h>
#include <spark/AppProvider.h>

#include "TemplateAppComponentMapInitializer.h"

using namespace spark;
using namespace masl;

/////////////////////////////////////////////////////////////////////////App-Instance
#ifdef __ANDROID__
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    spark::AppProvider::get().setApp(masl::Ptr<templateapp::TemplateApp>(new templateapp::TemplateApp()));
    return JNI_VERSION_1_6;
}
#endif


/////////////////// Application code, this should be in java or script language later...
namespace templateapp {

    TemplateApp::TemplateApp():BaseApp("TemplateApp") {
    }

    TemplateApp::~TemplateApp() {
    }

    void TemplateApp::setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, int theScreenWidth, int theScreenHeight) {
        BaseApp::setup(theCurrentMillis, theAssetPath, theScreenWidth, theScreenHeight);
        TemplateAppComponentMapInitializer::init();
        loadLayoutAndRegisterEvents("/main", theScreenWidth, theScreenHeight);
    }

}


