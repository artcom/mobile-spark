
#include "BaseApp.h"

#include <masl/Logger.h>
#include <animation/AnimationManager.h>

#ifdef __ANDROID__
#include <android/AndroidAssetProvider.h>
#endif

#include "SparkComponentFactory.h"

namespace spark {

    BaseApp::BaseApp(): _myAnimate(true) {
    }

    BaseApp::~BaseApp() {
    }

    bool BaseApp::setup(std::string assetPath, std::string layoutFile) {
        AC_PRINT << "setup";

//TODO: AssetProvider for ios?
#ifdef __ANDROID__
        assetProvider = android::AndroidAssetProviderPtr(new android::AndroidAssetProvider(assetPath));
#endif
        //load layout
        window = boost::static_pointer_cast<spark::Window>(SparkComponentFactory::get().loadSparkLayout(BaseAppPtr(this), layoutFile));
        return true;
    }
    void BaseApp::onFrame(const long theCurrentMillis) {
        //AC_PRINT << "onFrame " << theCurrentMillis;
        if (_myAnimate) {
            animation::AnimationManager::get().doFrame(theCurrentMillis);
        }
        window->render();
    }

    void BaseApp::onTouch() {
        AC_PRINT << "BaseApp::onTOUCH";
        //_myAnimate = !_myAnimate;
    }
}

