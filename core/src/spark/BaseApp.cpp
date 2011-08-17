
#include "BaseApp.h"

#include <masl/Logger.h>

#ifdef __ANDROID__
#include <android/AndroidAssetProvider.h>
#endif

#include "SparkComponentFactory.h"

namespace spark {

    BaseApp::BaseApp(): step(0), _myAnimate(true) {
    }

    BaseApp::~BaseApp() {
    }

    bool BaseApp::setup(std::string assetPath, std::string layoutFile) {
        AC_PRINT << "setup";

//TODO: AssetProvider for ios?
#ifdef __ANDROID__
        AC_PRINT << "android?"
        assetProvider = android::AndroidAssetProviderPtr(new android::AndroidAssetProvider(assetPath));
#endif
        AC_PRINT << "1";
        ComponentPtr myComponentPtr = SparkComponentFactory::get().loadSparkLayout(BaseAppPtr(this), layoutFile);
        //load layout
        AC_PRINT << "2";
        window = boost::static_pointer_cast<spark::Window>(myComponentPtr);
        AC_PRINT << "3";
        return true;
    }
    void BaseApp::onFrame() {
        //AC_PRINT << "onFrame";
        if (_myAnimate) {
            step += 0.0001f;
            if (step > 1.0f) {
                step = 0.0f;
            }
        }

        window->render();
    }

    void BaseApp::onTouch() {
        AC_PRINT << "onTOUCH";
        _myAnimate = !_myAnimate;
    }
}

