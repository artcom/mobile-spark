
#include "BaseApp.h"

#include <boost/enable_shared_from_this.hpp>

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
#include "Visitors.h"

using namespace mar;

namespace spark {

    void testFreeFunctionEvent(EventPtr theEvent) {
        AC_PRINT<<"testFreeFunctionEvent "<<theEvent->getType();
    }

    BaseApp::BaseApp() {
    }

    BaseApp::~BaseApp() {
    }

    bool BaseApp::setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, const std::string & theLayoutFile) {
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
        _mySparkWindow = boost::static_pointer_cast<spark::Window>(SparkComponentFactory::get().loadSparkLayoutFromFile(BaseAppPtr(this), theLayoutFile));

        //register for events
        spark::EventCallbackPtr myFrameCB = EventCallbackPtr(new MemberFunctionEventCallback<BaseApp, BaseAppPtr > ( shared_from_this(), &BaseApp::onFrame));
        _mySparkWindow->addEventListener(StageEvent::FRAME, myFrameCB);
        spark::EventCallbackPtr myCB = EventCallbackPtr(new MemberFunctionEventCallback<Window, WindowPtr>( _mySparkWindow, &Window::onTouch));
        _mySparkWindow->addEventListener(TouchEvent::TAP, myCB);
        spark::EventCallbackPtr myFreeCB = EventCallbackPtr(new FreeFunctionEventCallback(testFreeFunctionEvent));
        _mySparkWindow->addEventListener(TouchEvent::TAP, myFreeCB);

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
    
    void BaseApp::onEvent(std::string theEventString) {
        EventPtr myEvent = spark::EventFactory::get().handleEvent(theEventString);
        myEvent->connect(_mySparkWindow);
        (*myEvent)();
    }
    
    void BaseApp::onPause() {
        if (_mySparkWindow) {
            OnPauseComponentVisitor myVisitor;
            visitComponents(myVisitor, _mySparkWindow);   
        }
    }

    void BaseApp::onResume() {
        if (_mySparkWindow) {
            OnResumeComponentVisitor myVisitor;
            visitComponents(myVisitor, _mySparkWindow);
        }
    }
    
    void BaseApp::onFrame(EventPtr theEvent) {
        StageEventPtr myEvent = boost::static_pointer_cast<StageEvent>(theEvent);
        animation::AnimationManager::get().doFrame(myEvent->getCurrentTime());
        _myGLCanvas->preRender(_mySparkWindow->getClearColor());
        _mySparkWindow->render();
    }

}

