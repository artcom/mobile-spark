// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __


#include "BaseApp.h"

#include <sys/stat.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/progress.hpp>

#include <masl/AssetProvider.h>
#include <masl/AudioEngine.h>
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
    #include <android/AndroidMovieEngine.h>
#endif
#ifdef iOS
    #include <ios/IOSAssetProvider.h>
    #include <ios/IOSAudioEngine.h>
    #include <ios/IOSMovieEngine.h>
    #include <ios/IOSMobileSDK.h>
#endif

#include "SparkComponentFactory.h"
#include "Window.h"
#include "EventFactory.h"
#include "Visitors.h"
#include "ComponentMapInitializer.h"

using namespace mar;
using namespace masl;

#define DB(x) //x

namespace spark {


    BaseApp::BaseApp(const std::string & theAppPath) : appPath_(theAppPath),
        _mySetupFlag(false), _myOneBaseAppOnFrameCallPerRenderloopFlag(false){
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
#ifdef ANDROID
        assetProviderSetup(theAssetPath, appPath_);
#endif
#ifdef iOS
        MobileSDK_Singleton::get().setMobileSDK(ios::IOSMobileSDKPtr(new ios::IOSMobileSDK()));
#endif
    }

    void BaseApp::loadLayoutAndRegisterEvents(int theScreenWidth, int theScreenHeight, const std::string & theBaseName) {
        bool dummy;
        std::string myLayoutFile = findBestMatchedLayout(theBaseName, theScreenWidth, theScreenHeight, dummy);
        //load layout
        _mySparkWindow = boost::static_pointer_cast<spark::Window>(SparkComponentFactory::get().loadSparkComponentsFromFile(shared_from_this(), myLayoutFile));
        _mySparkWindow->realize();
        _mySparkWindow->setSize(theScreenWidth,theScreenHeight);
        //register for events
        spark::EventCallbackPtr myFrameCB = EventCallbackPtr(new MemberFunctionEventCallback<BaseApp, BaseAppWeakPtr > ( BaseAppWeakPtr(BaseAppPtr(shared_from_this())), &BaseApp::onFrame));
        spark::EventCallbackPtr myBackButtonCB = EventCallbackPtr(new MemberFunctionEventCallback<BaseApp, BaseAppWeakPtr > ( BaseAppWeakPtr(BaseAppPtr(shared_from_this())), &BaseApp::onBackButton));
        _mySparkWindow->addEventListener(StageEvent::FRAME, myFrameCB);

        spark::EventCallbackPtr myCB = EventCallbackPtr(new MemberFunctionEventCallback<Window, WindowWeakPtr>( _mySparkWindow, &Window::onTouch));
        _mySparkWindow->addEventListener(TouchEvent::TAP, myCB);
        _mySparkWindow->addEventListener(TouchEvent::DOUBLETAP, myCB);
        _mySparkWindow->addEventListener(TouchEvent::LONGPRESS, myCB);
        _mySparkWindow->addEventListener(TouchEvent::DOWN, myCB);
        _mySparkWindow->addEventListener(TouchEvent::UP, myCB);
        _mySparkWindow->addEventListener(ApplicationEvent::BUTTON_BACK, myBackButtonCB);
    }

    void BaseApp::onEvent(const std::string & theEventString) {
        DB(AC_TRACE << "a string event came in :" << theEventString);
        EventPtr myEvent = spark::EventFactory::get().createEvent(theEventString);
        if (myEvent) {
            boost::mutex::scoped_lock lock(_myEventMutex);
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

    void BaseApp::onBackButton(EventPtr theEvent) {
        exit();
    }

    void BaseApp::exit() {
        masl::MobileSDK_Singleton::get().getNative()->exit();
    }

    void BaseApp::handleEvents() {
        DB(AC_TRACE << "-----------------------------------------------");
        _myOneBaseAppOnFrameCallPerRenderloopFlag = false;
        // ------------------------------ handle event strategy -------------------------------------------------
        // do not delay or ignore systemrelevant events
        // handle the first incoming type of classtype and type events
        // delay second incoming events of same classtype
        // (i.e. events:['down', 'tap'] are class type 'GestureEvent' but different type, delay 'tap'
        // ignore second incoming event of same classtype and type
        // (i.e. events:['orientation', 'frame', 'orientation', 'frame'], ignore second 'frame' and 'orientation'
        // delay touch up events until we have no more touch or gesture events in our queue
        // -------------------------------------------------------------------------------------------------------
        boost::mutex::scoped_lock lock(_myEventMutex);
        EventPtrList myDelayedEvents;
        std::map<std::string, bool> myDelayEventFilter;
        std::map<std::string, bool> myIgnoreEventFilter;
        int myHandledEventCounter = 0;
        boost::timer::timer myTimer;
        DB(AC_TRACE << "########################################Base App handle Events " << _myEvents.size());
        int i = 0;
        bool myGestureOrTouchEvents = false;
        for (EventPtrList::iterator it = _myEvents.begin(); it != _myEvents.end(); ++it) {
            bool myCurrentEventisTouchUp = (*it)->classname_()== TouchEvent::CLASSNAME  && (*it)->getType() == TouchEvent::UP;
            if ( ((*it)->classname_()== TouchEvent::CLASSNAME ||  (*it)->classname_()== GestureEvent::CLASSNAME) && !myCurrentEventisTouchUp) {
                myGestureOrTouchEvents = true;
                break;
            }
        }
        for (EventPtrList::iterator it = _myEvents.begin(); it != _myEvents.end(); ++it) {
            DB(AC_TRACE << "EVENT# " << i);
            bool myIgnoreEventFlag = false;
            bool myDelayEventFlag = false;
            if (!(*it)->isSystemRelevant()) {
                // allow only one type per frame and therefore do not delay it
                if (myIgnoreEventFilter.find( (*it)->getType()) != myIgnoreEventFilter.end() ) {
                    myIgnoreEventFlag = true;
                }
                if (myDelayEventFilter.find( (*it)->classname_()) != myDelayEventFilter.end() ) {
                    myDelayEventFlag = true;
                }
                bool myCurrentEventisTouchUp = (*it)->classname_()== TouchEvent::CLASSNAME  && (*it)->getType() == TouchEvent::UP;
                if (myCurrentEventisTouchUp && myGestureOrTouchEvents){
                    myDelayEventFlag = true;
                }
            }
            if (!myIgnoreEventFlag && !myDelayEventFlag) {
                myIgnoreEventFilter[(*it)->getType()] = true;
                myDelayEventFilter[(*it)->classname_()] = true;
                (*it)->connect(_mySparkWindow);
                DB(AC_TRACE << "handle event: " << (*(*it)));
                (*(*it))();
                myHandledEventCounter++;
            } else {
                if (myDelayEventFlag && !myIgnoreEventFlag) {
                    DB(AC_TRACE << "delay event: " << (*it)->getType());
                    myDelayedEvents.push_back((*it));
                } else {
                    DB(AC_TRACE << "ignore event: " << (*it)->getType());
                }
            }
            ++i;
        }
        //_myEvents.clear();
        _myEvents = myDelayedEvents;
        DB(AC_TRACE << "################ handle events finished " << myHandledEventCounter);
        DB(AC_TRACE << "handleEvents duration " << myTimer.elapsed() << " ms");
    }


    void BaseApp::onFrame(EventPtr theEvent) {
        if (!_myOneBaseAppOnFrameCallPerRenderloopFlag) {
            boost::timer::timer myTimer;
            DB(AC_TRACE << "onFrame");
            StageEventPtr myEvent = boost::static_pointer_cast<StageEvent>(theEvent);
            animation::AnimationManager::get().doFrame(myEvent->getCurrentTime());
            _mySparkWindow->renderWindow();
            DB(AC_TRACE << "onFrame done, currentTime "<< myEvent->getCurrentTime());
            DB(AC_TRACE << "OnFrame duration " << myTimer.elapsed() << " s");
        }
        _myOneBaseAppOnFrameCallPerRenderloopFlag = true;
    }


    std::string
    findBestMatchedLayout(const std::string & theBaseName, int theScreenWidth, int theScreenHeight, bool &isPortrait) {
        AC_INFO << "......... findBestMatchedLayout for baseName: " << theBaseName << " with screen resolution: " << theScreenWidth << "/" << theScreenHeight;
        std::vector<std::string> myFiles = AssetProviderSingleton::get().ap()->getFilesFromPath("layouts/", theBaseName);
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
                std::string myChoice = getFilenamePart(myFiles[i]);
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
                    AC_INFO << "Excellent we have a layout<->device match -> use layout '" << myBestLayoutName << "' file: '" << myBestMatch << "'";
                    AC_INFO << "Layout : "<< myBestLayoutWidth << "x" <<  myBestLayoutHeight;
                    break;
                }
            }
        }
        if (!myExactMatchFlag) {
            AC_INFO << "Sorry, we did not find layout<->device match -> use default layout '" << myBestLayoutName << "' file: '" << myBestMatch << "'";
            AC_INFO << "Layout : "<< myBestLayoutWidth << "x" <<  myBestLayoutHeight;
        }
        return myBestMatch;
    }

    void assetProviderSetup(const std::string & theAssetPath, const std::string & theAppPath ) {
#ifdef iOS
        masl::AssetProviderSingleton::get().setAssetProvider(ios::IOSAssetProviderPtr(new ios::IOSAssetProvider(theAssetPath, theAppPath)));
        masl::AudioEngineSingleton::get().setAudioEngine(ios::IOSAudioEnginePtr(new ios::IOSAudioEngine()));
        masl::MovieEngineSingleton::get().setMovieEngine(ios::IOSMovieEnginePtr(new ios::IOSMovieEngine()));
#elif ANDROID
        masl::AssetProviderSingleton::get().setAssetProvider(android::AndroidAssetProviderPtr(new android::AndroidAssetProvider(theAssetPath, theAppPath)));
        masl::AudioEngineSingleton::get().setAudioEngine(android::AndroidAudioEnginePtr(new android::AndroidAudioEngine("com/artcom/mobile/Base/AudioDelegate")));
        masl::MovieEngineSingleton::get().setMovieEngine(android::AndroidMovieEnginePtr(new android::AndroidMovieEngine("com/artcom/mobile/Base/VideoDelegate")));
        mkdir(std::string(masl::AssetProviderSingleton::get().ap()->getAssetPath()).c_str(), 755);
        mkdir(std::string(masl::AssetProviderSingleton::get().ap()->getAssetPath() + "/downloads/").c_str(), 755);
#endif
#ifdef iOS
       // masl::AssetProviderSingleton::get().ap()->addIncludePath("");
        masl::AssetProviderSingleton::get().ap()->addIncludePath("../../Documents/");
        masl::AssetProviderSingleton::get().ap()->addIncludePath("../../Documents/downloads/");
#elif ANDROID
        masl::AssetProviderSingleton::get().ap()->addIncludePath("downloads/");
#endif
        masl::AssetProviderSingleton::get().ap()->addIncludePath("");
        masl::AssetProviderSingleton::get().ap()->addIncludePath("textures/");
        masl::AssetProviderSingleton::get().ap()->addIncludePath("layouts/");
        masl::AssetProviderSingleton::get().ap()->addIncludePath("shaders/");
        masl::AssetProviderSingleton::get().ap()->addIncludePath("models/");
        masl::AssetProviderSingleton::get().ap()->addIncludePath("fonts/");
        masl::AssetProviderSingleton::get().ap()->addIncludePath("sounds/");


    }

}

