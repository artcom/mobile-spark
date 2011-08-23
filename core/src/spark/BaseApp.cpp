
#include "BaseApp.h"

#include <masl/Logger.h>
#include <animation/AnimationManager.h>

#ifdef __ANDROID__
    #include <android/AndroidAssetProvider.h>
#endif
#if __APPLE__
    #include <ios/IOSAssetProvider.h>
#endif

#include "SparkComponentFactory.h"

using namespace mar;

namespace spark {

    BaseApp::BaseApp(): _myAnimate(true) {
    }

    BaseApp::~BaseApp() {
    }

    bool BaseApp::setup(const long theCurrentMillis, const std::string & theAssetPath, const std::string & theLayoutFile) {
        //AC_PRINT << "setup";
        //init animationManager with setup-time 
        //(needed for animations created on setup)
        animation::AnimationManager::get().init(theCurrentMillis);

#ifdef __ANDROID__
        assetProvider = android::AndroidAssetProviderPtr(new android::AndroidAssetProvider(theAssetPath));
#endif
#if __APPLE__
        assetProvider = ios::IOSAssetProviderPtr(new ios::IOSAssetProvider(theAssetPath));
#endif
        //load layout
        _mySparkWindow = boost::static_pointer_cast<spark::Window>(SparkComponentFactory::get().loadSparkLayout(BaseAppPtr(this), theLayoutFile));
            
        _myGLCanvas = CanvasPtr( new Canvas());
        _myGLCanvas->initGLState();
        return true;
    }

    void BaseApp::onSizeChanged(int theWidth, int theHeight) {
        AC_PRINT << "BaseApp::onSizeChanged(int theWidth, int theHeight) : " << theWidth << "x" << theHeight;
        if (_mySparkWindow) {
            _mySparkWindow->onSizeChanged(theWidth, theHeight);
        }
    }
    
    void BaseApp::onFrame(const long theCurrentMillis) {
        //AC_PRINT << "onFrame " << theCurrentMillis;
        if (_myAnimate) {
            animation::AnimationManager::get().doFrame(theCurrentMillis);
        }
        _myGLCanvas->preRender(_mySparkWindow->getClearColor());
        _mySparkWindow->render();
    }

    void BaseApp::onTouch() {
        AC_PRINT << "BaseApp::onTOUCH";
        //_myAnimate = !_myAnimate;
    }
}

