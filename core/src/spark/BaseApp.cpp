
#include "BaseApp.h"

#include <sys/stat.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/progress.hpp>

#include <masl/AssetProvider.h>
#include <masl/AudioEngine.h>
#include <masl/AutoLocker.h>
#include <masl/BaseEntry.h>
#include <masl/CallStack.h>
#include <masl/Exception.h>
#include <masl/file_functions.h>
#include <masl/Logger.h>
#include <masl/MobileSDK.h>
#include <masl/signal_functions.h>
#include <masl/string_functions.h>
#include <masl/XMLNode.h>
#include <masl/XMLUtils.h>

#include <animation/AnimationManager.h>

#include <mar/TextureLoader.h>


#ifdef ANDROID
    #include <android/AndroidAssetProvider.h>
    #include <android/AndroidAudioEngine.h>
    #include <android/AndroidMobileSDK.h>
#endif
#ifdef iOS
    #include <ios/IOSAssetProvider.h>
    #include <ios/IOSAudioEngine.h>
    #include <ios/IOSMobileSDK.h>
#endif

#include "SparkComponentFactory.h"
#include "Window.h"
#include "EventFactory.h"
#include "Visitors.h"
#include "ComponentMapInitializer.h"

using namespace mar;
using namespace masl;

namespace spark {


    BaseApp::BaseApp(const std::string & theAppPath) : appPath_(theAppPath), 
        _mySetupFlag(false) {
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
        assetProviderSetup(theAssetPath, appPath_);
#ifdef iOS
        MobileSDK_Singleton::get().setMobileSDK(ios::IOSMobileSDKPtr(new ios::IOSMobileSDK()));
#endif
    }

    void BaseApp::loadLayoutAndRegisterEvents(const std::string & theBaseName, int theScreenWidth, int theScreenHeight) {
        bool dummy;
        std::string myLayoutFile = findBestMatchedLayout(theBaseName, theScreenWidth, theScreenHeight, dummy);
        //load layout
        _mySparkWindow = boost::static_pointer_cast<spark::Window>(SparkComponentFactory::get().loadSparkComponentsFromFile(shared_from_this(), myLayoutFile));
        _mySparkWindow->realize();
        _mySparkWindow->setSize(theScreenWidth,theScreenHeight);
        //register for events
        spark::EventCallbackPtr myFrameCB = EventCallbackPtr(new MemberFunctionEventCallback<BaseApp, BaseAppWeakPtr > ( BaseAppWeakPtr(BaseAppPtr(shared_from_this())), &BaseApp::onFrame));
        _mySparkWindow->addEventListener(StageEvent::FRAME, myFrameCB);
            
        spark::EventCallbackPtr myCB = EventCallbackPtr(new MemberFunctionEventCallback<Window, WindowWeakPtr>( _mySparkWindow, &Window::onTouch));
        _mySparkWindow->addEventListener(TouchEvent::TAP, myCB);
        _mySparkWindow->addEventListener(TouchEvent::LONGPRESS, myCB);
            
    }

    void BaseApp::onEvent(const std::string & theEventString) {
        AC_TRACE << "a string event came in :" << theEventString;
        EventPtr myEvent = spark::EventFactory::get().createEvent(theEventString);
        if (myEvent) {
            AutoLocker<ThreadLock> myLocker(_myLock);        
            _myEvents.push_back(myEvent);
        }
    }

    void BaseApp::onPause() {
        AC_DEBUG << "BaseApp::onPause";
        mar::TextureLoader::get().clear();
        if (_mySparkWindow) {
            OnPauseComponentVisitor myVisitor;
            parentFirstVisitComponents(myVisitor, _mySparkWindow);
        }
    }

    void BaseApp::onResume() {
        AC_DEBUG << "BaseApp::onResume";
        if (_mySparkWindow) {
            OnResumeComponentVisitor myVisitor;
            childFirstVisitComponents(myVisitor, _mySparkWindow);
        }
    }
    
    void BaseApp::handleEvents() {
        AutoLocker<ThreadLock> myLocker(_myLock);        
        boost::timer::timer myTimer;
        AC_TRACE << "########################################Base App handle Events " << _myEvents.size();
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
        AC_TRACE << "handleEvents duration " << myTimer.elapsed() << " s";
    }
    

    void BaseApp::onFrame(EventPtr theEvent) {
        boost::timer::timer myTimer;
        AC_TRACE << "onFrame";
        StageEventPtr myEvent = boost::static_pointer_cast<StageEvent>(theEvent);
        animation::AnimationManager::get().doFrame(myEvent->getCurrentTime());
        _mySparkWindow->render();
        AC_TRACE << "onFrame done, currentTime "<< myEvent->getCurrentTime();
        AC_TRACE << "OnFrame duration " << myTimer.elapsed() << " s";
    }
    
   
    std::string
    findBestMatchedLayout(const std::string & theBaseName, int theScreenWidth, int theScreenHeight, bool &isPortrait) {
        AC_PRINT << "......... findBestMatchedLayout for baseName: " << theBaseName << " with screen resolution: " << theScreenWidth << "/" << theScreenHeight;
        std::vector<std::string> myFiles = AssetProviderSingleton::get().ap()->getFilesFromPath(theBaseName);
        int myScreensLargerSide = theScreenWidth > theScreenHeight ? theScreenWidth : theScreenHeight;
        int myScreensSmallerSide = myScreensLargerSide ==  theScreenHeight ? theScreenWidth : theScreenHeight;
        std::string myBestMatch = "";
        std::string myBestLayoutName = "";
        int myBestLayoutWidth = 0;
        int myBestLayoutHeight = 0;
        bool myPortrait = false;
        
        bool myExactMatchFlag = false;
        int myLayoutWidth = 0;
        int myLayoutHeight = 0;
        std::string myLayoutName = "";
        for (unsigned int i = 0; i < myFiles.size(); i++) {
            if (getExtension(myFiles[i]) == "spark") {
                std::string myChoice = getDirectoryPart(theBaseName) + getFilenamePart(myFiles[i]);
                std::string myLayout = masl::AssetProviderSingleton::get().ap()->getStringFromFile(myChoice);
                XMLNodePtr myNode(new XMLNode(myLayout));
                if (myNode->nodeName == "Window") {
                    for (std::map<std::string, std::string>::iterator it = myNode->attributes.begin(); it != myNode->attributes.end(); ++it) {
                        if (it->first == "name") {
                            myLayoutName = it->second;
                        } else if (it->first == "width") {
                            myLayoutWidth = as<int>(it->second);
                        } else if (it->first == "height") {
                            myLayoutHeight = as<int>(it->second);
                        } else if (it->first == "orientation") {
                            myPortrait = "portrait" == (it->second);
                        } 
                    }
                }
                if (i == 0) {
                    myBestMatch = myChoice;
                    myBestLayoutName = myLayoutName;
                    myBestLayoutWidth = myLayoutWidth;
                    myBestLayoutHeight = myLayoutHeight;
                    isPortrait = myPortrait;
                }
                int myLayoutsLargerSide = myLayoutWidth > myLayoutHeight ? myLayoutWidth : myLayoutHeight;
                int myLayoutsSmallerSide = myScreensLargerSide ==  myLayoutHeight ? myLayoutWidth : myLayoutHeight;
                if (myLayoutsLargerSide == myScreensLargerSide && myLayoutsSmallerSide == myScreensSmallerSide ) {
                    myBestMatch = myChoice;
                    myBestLayoutName = myLayoutName;
                    myBestLayoutWidth = myLayoutWidth;
                    myBestLayoutHeight = myLayoutHeight;
                    isPortrait = myPortrait;
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

    void assetProviderSetup(const std::string & theAssetPath, const std::string & theAppPath ) {
#ifdef iOS
        masl::AssetProviderSingleton::get().setAssetProvider(ios::IOSAssetProviderPtr(new ios::IOSAssetProvider(theAssetPath, theAppPath)));
        masl::AudioEngineSingleton::get().setAudioEngine(ios::IOSAudioEnginePtr(new ios::IOSAudioEngine()));
#elif ANDROID
        masl::AssetProviderSingleton::get().setAssetProvider(android::AndroidAssetProviderPtr(new android::AndroidAssetProvider(theAssetPath, theAppPath)));
        masl::AudioEngineSingleton::get().setAudioEngine(android::AndroidAudioEnginePtr(new android::AndroidAudioEngine("com/artcom/mobile/Base/SparkViewerActivity")));
        mkdir(std::string(masl::AssetProviderSingleton::get().ap()->getAssetPath() + "/downloads/").c_str(), 755);
#endif
        masl::AssetProviderSingleton::get().ap()->addIncludePath("core/shaders/");
        masl::AssetProviderSingleton::get().ap()->addIncludePath(theAppPath + "/");
        masl::AssetProviderSingleton::get().ap()->addIncludePath(theAppPath + "/textures/");
        masl::AssetProviderSingleton::get().ap()->addIncludePath(theAppPath + "/layouts/");
        masl::AssetProviderSingleton::get().ap()->addIncludePath(theAppPath + "/shaders/");
        masl::AssetProviderSingleton::get().ap()->addIncludePath(theAppPath + "/models/");
        masl::AssetProviderSingleton::get().ap()->addIncludePath(theAppPath + "/fonts/");
        masl::AssetProviderSingleton::get().ap()->addIncludePath(theAppPath + "/sounds/");
#ifdef iOS
        masl::AssetProviderSingleton::get().ap()->addIncludePath("/../Documents/");
        masl::AssetProviderSingleton::get().ap()->addIncludePath("/../Documents/downloads/");
#elif ANDROID
        masl::AssetProviderSingleton::get().ap()->addIncludePath(theAppPath + "/downloads/");
#endif

    }
    
}

