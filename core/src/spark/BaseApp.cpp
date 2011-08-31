
#include "BaseApp.h"

#include <masl/Logger.h>
#include <masl/BaseEntry.h>
#include <masl/XMLNode.h>

#include <mar/AssetProvider.h>
#include <animation/AnimationManager.h>

#ifdef __ANDROID__
    #include <android/AndroidAssetProvider.h>
#endif
#if __APPLE__
    #include <ios/IOSAssetProvider.h>
#endif

#include "SparkComponentFactory.h"
#include "EventFactory.h"

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
        AssetProviderSingleton::get().setAssetProvider(android::AndroidAssetProviderPtr(new android::AndroidAssetProvider(theAssetPath)));
#endif
#if __APPLE__
        AssetProviderSingleton::get().setAssetProvider(ios::IOSAssetProviderPtr(new ios::IOSAssetProvider(theAssetPath)));
#endif
        //load layout
        _mySparkWindow = boost::static_pointer_cast<spark::Window>(SparkComponentFactory::get().loadSparkLayout(BaseAppPtr(this), theLayoutFile));
        spark::EventCallbackPtr myCB(new spark::EventCallback( _mySparkWindow, &Component::testEvent));
        _mySparkWindow->addEventListener("TouchEvent", myCB);
            
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
    
    void BaseApp::onPause() {
        if (_mySparkWindow) {
            _mySparkWindow->onPause();
        }
    }

    void BaseApp::onEvent(std::string theEventString) {
        EventPtr myEvent = spark::EventFactory::get().handleEvent(theEventString);
        myEvent->connect(_mySparkWindow);
        (*myEvent)();
    }
    
    void BaseApp::onResume() {
        if (_mySparkWindow) {
            _mySparkWindow->onResume();
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

