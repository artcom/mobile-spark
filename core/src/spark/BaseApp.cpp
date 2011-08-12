
#include "BaseApp.h"

#include <masl/Logger.h>
#include <masl/XMLUtils.h>
#include <mar/openGL_functions.h>
#include <spark/SparkComponentFactory.h>

#ifdef __ANDROID__
#include <android/AndroidAssetProvider.h>
#endif

namespace spark {

    BaseApp::BaseApp(): step(0), _myAnimate(true) {
    }

    BaseApp::~BaseApp() {
    }

    bool BaseApp::setup(std::string assetPath, std::string layoutFile) {
        AC_PRINT << "setup";

        printGLInfo();
    

//TODO: AssetProvider for ios?
#ifdef __ANDROID__
        assetProvider = android::AndroidAssetProviderPtr(new android::AndroidAssetProvider(assetPath));
#endif

        //load layout
        window = boost::static_pointer_cast<spark::Window>(spark::SparkComponentFactory::loadSparkLayout(BaseAppPtr(this), layoutFile));
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

