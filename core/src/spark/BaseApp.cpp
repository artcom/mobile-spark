
#include "BaseApp.h"

#include <boost/enable_shared_from_this.hpp>

#include <masl/Logger.h>
#include <masl/BaseEntry.h>
#include <masl/XMLNode.h>
#include <masl/MobileSDK.h>
#include <masl/XMLUtils.h>
#include <masl/file_functions.h>
#include <masl/signal_functions.h>
#include <masl/string_functions.h>
#include <masl/Exception.h>
#include <masl/Auto.h>
#include <masl/CallStack.h>

#include <mar/AssetProvider.h>
#include <animation/AnimationManager.h>

#ifdef __ANDROID__
    #include <android/AndroidAssetProvider.h>
    #include <android/AndroidMobileSDK.h>
#endif
#if __APPLE__
    #include <ios/IOSAssetProvider.h>
    #include <ios/IOSMobileSDK.h>
#endif

#include "SparkComponentFactory.h"
#include "EventFactory.h"
#include "Visitors.h"
#include "ComponentMapInitializer.h"

using namespace mar;
using namespace masl;

namespace spark {


    BaseApp::BaseApp(const std::string & theAppPath) : appPath_(theAppPath), 
        _myChooseLayoutFlag(false), _mySetupFlag(false), _mySparkRealizedFlag(false) {
        masl::initSignalHandling();
    }

    BaseApp::~BaseApp() {
        // this has to be done only ONCE when exiting
        xmlCleanupParser();
        xmlMemoryDump();

    }

    void BaseApp::setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, int theScreenWidth, int theScreenHeight) {
        _mySetupFlag = true;
        ComponentMapInitializer::init();

        //init animationManager with setup-time
        //(needed for animations created on setup)
        animation::AnimationManager::get().init(theCurrentMillis);

#ifdef __ANDROID__
        AssetProviderSingleton::get().setAssetProvider(android::AndroidAssetProviderPtr(new android::AndroidAssetProvider(theAssetPath)));
#endif
#if __APPLE__
        AssetProviderSingleton::get().setAssetProvider(ios::IOSAssetProviderPtr(new ios::IOSAssetProvider(theAssetPath)));
        MobileSDK_Singleton::get().setMobileSDK(ios::IOSMobileSDKPtr(new ios::IOSMobileSDK()));
#endif
        AssetProviderSingleton::get().ap()->addIncludePath("core/shaders/");
        AssetProviderSingleton::get().ap()->addIncludePath(appPath_ + "/textures");
        AssetProviderSingleton::get().ap()->addIncludePath(appPath_ + "/layouts");
        AssetProviderSingleton::get().ap()->addIncludePath(appPath_ + "/shaders");
        AssetProviderSingleton::get().ap()->addIncludePath(appPath_ + "/models");
        AssetProviderSingleton::get().ap()->addIncludePath(appPath_ + "/fonts");
        AssetProviderSingleton::get().ap()->addIncludePath(appPath_);
    }

    void BaseApp::realize() {
        _mySparkRealizedFlag = true;
        RealizeComponentVisitor myVisitor;
        visitComponents(myVisitor, _mySparkWindow);
        I18nComponentVisitor myI18nVisitor;
        visitComponents(myI18nVisitor, _mySparkWindow);
    }

    std::string
    BaseApp::findBestMatchedLayout(std::string theBaseName, int theScreenWidth, int theScreenHeight) {
        std::vector<std::string> myFiles = AssetProviderSingleton::get().ap()->getFilesFromPath(theBaseName);
        int myScreensLargerSide = theScreenWidth > theScreenHeight ? theScreenWidth : theScreenHeight;
        int myScreensSmallerSide = myScreensLargerSide ==  theScreenHeight ? theScreenWidth : theScreenHeight;
        std::string myBestMatch = "";
        std::string myBestLayoutName = "";
        int myBestLayoutWidth = 0;
        int myBestLayoutHeight = 0;

        bool myExactMatchFlag = false;
        int myLayoutWidth = 0;
        int myLayoutHeight = 0;
        std::string myLayoutName = "";
        for (unsigned int i = 0; i < myFiles.size(); i++) {
            if (getExtension(myFiles[i]) == "spark") {
                std::string myChoice = getDirectoryPart(theBaseName) + getFilenamePart(myFiles[i]);
                std::string myLayout = AssetProviderSingleton::get().ap()->getStringFromFile(myChoice);
                XMLNodePtr myNode(new XMLNode(myLayout));
                if (myNode->nodeName == "Window") {
                    for (std::map<std::string, std::string>::iterator it = myNode->attributes.begin(); it != myNode->attributes.end(); ++it) {
                        if (it->first == "name") {
                            myLayoutName = it->second;
                        }
                        if (it->first == "width") {
                            myLayoutWidth = as<int>(it->second);
                        }
                        if (it->first == "height") {
                            myLayoutHeight = as<int>(it->second);
                        }
                    }
                }
                if (i == 0) {
                    myBestMatch = myChoice;
                    myBestLayoutName = myLayoutName;
                    myBestLayoutWidth = myLayoutWidth;
                    myBestLayoutHeight = myLayoutHeight;
                }
                int myLayoutsLargerSide = myLayoutWidth > myLayoutHeight ? myLayoutWidth : myLayoutHeight;
                int myLayoutsSmallerSide = myScreensLargerSide ==  myLayoutHeight ? myLayoutWidth : myLayoutHeight;
                //AC_PRINT << "check layout '" << myLayoutName << " width " << myLayoutWidth << "'" << " height " << myLayoutHeight << " screen :" << theScreenWidth << " x" << theScreenHeight;
                if (myLayoutsLargerSide == myScreensLargerSide && myLayoutsSmallerSide == myScreensSmallerSide ) {
                    myBestMatch = myChoice;
                    myBestLayoutName = myLayoutName;
                    myBestLayoutWidth = myLayoutWidth;
                    myBestLayoutHeight = myLayoutHeight;
                    myExactMatchFlag = true;
                    AC_PRINT << "Excellent we have a layout<->device match -> use layout '" << myBestLayoutName << "' file: '" << myBestMatch << "'";
                    AC_PRINT << "Layout : "<< myBestLayoutWidth << "x" <<  myBestLayoutHeight;
                    break;
                }
            }
        }
        if (!myExactMatchFlag) {
            AC_PRINT << "Sorry, we did not find layout<->device match -> use default layout '" << myBestLayoutName << "' file: '" << myBestMatch << "'";
            AC_PRINT << "Layout : "<< myBestLayoutWidth << "x" <<  myBestLayoutHeight;
        }
        return myBestMatch;
    }

    void
    BaseApp::loadLayoutAndRegisterEvents(const std::string & theBaseName, int theScreenWidth, int theScreenHeight) {
        std::string myLayoutFile = "";
        if (_myChooseLayoutFlag) {
            myLayoutFile = findBestMatchedLayout(theBaseName, theScreenWidth, theScreenHeight);
        }  else {
            myLayoutFile = theBaseName + ".spark";
        }
        //load layout
        _mySparkWindow = boost::static_pointer_cast<spark::Window>(SparkComponentFactory::get().loadSparkComponentsFromFile(BaseAppPtr(this), myLayoutFile));

        //register for events
        spark::EventCallbackPtr myFrameCB = EventCallbackPtr(new MemberFunctionEventCallback<BaseApp, BaseAppPtr > ( shared_from_this(), &BaseApp::onFrame));
        _mySparkWindow->addEventListener(StageEvent::FRAME, myFrameCB);
        spark::EventCallbackPtr myCB = EventCallbackPtr(new MemberFunctionEventCallback<Window, WindowPtr>( _mySparkWindow, &Window::onTouch));
        _mySparkWindow->addEventListener(TouchEvent::TAP, myCB);
        spark::EventCallbackPtr myOnPauseCB = EventCallbackPtr(new MemberFunctionEventCallback<BaseApp, BaseAppPtr > ( shared_from_this(), &BaseApp::onPause));
        _mySparkWindow->addEventListener(StageEvent::PAUSE, myOnPauseCB);
    }

    void BaseApp::onEvent(std::string theEventString) {
        AC_TRACE << "a string event came in :" << theEventString;
        //masl::dumpstack();
        EventPtr myEvent = spark::EventFactory::get().createEvent(theEventString);
        if (myEvent) {
            AutoLocker<ThreadLock> myLocker(_myLock);        
            _myEvents.push_back(myEvent);
        }
    }

    void BaseApp::onPause(EventPtr theEvent) {
        if (_mySparkWindow) {
            OnPauseComponentVisitor myVisitor;
            visitComponents(myVisitor, _mySparkWindow);
        }
    }

    void BaseApp::onResume() {
        AC_TRACE << "onResume";
        if (_mySparkWindow) {
            OnResumeComponentVisitor myVisitor;
            visitComponents(myVisitor, _mySparkWindow);
        }
    }
    
    void BaseApp::handleEvents() {
        AC_TRACE << "########################################Base App handle Events " << _myEvents.size();
        AutoLocker<ThreadLock> myLocker(_myLock);        
        int i = 0;
        for (EventPtrList::iterator it = _myEvents.begin(); it != _myEvents.end(); ++it) {
            AC_TRACE << "EVENT# " << i;
            (*it)->connect(_mySparkWindow);
            AC_TRACE << "handle event: " << (*(*it));
            (*(*it))();
            ++i;
        }            
        _myEvents.clear();        
        AC_TRACE << "################ handle events finished " << _myEvents.size();
    }
    

    void BaseApp::onFrame(EventPtr theEvent) {
        AC_TRACE << "onFrame";
        StageEventPtr myEvent = boost::static_pointer_cast<StageEvent>(theEvent);
        animation::AnimationManager::get().doFrame(myEvent->getCurrentTime());
        _mySparkWindow->render();
        AC_TRACE << "onFrame done, currentTime "<< myEvent->getCurrentTime();
    }
}

