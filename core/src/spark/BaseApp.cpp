
#include "BaseApp.h"

#include <boost/enable_shared_from_this.hpp>

#include <masl/Logger.h>
#include <masl/BaseEntry.h>
#include <masl/XMLNode.h>
#include <masl/XMLUtils.h>
#include <masl/file_functions.h>

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
#include "ComponentMapInitializer.h"

using namespace mar;

namespace spark {


    BaseApp::BaseApp(const std::string & theAppPath) : appPath_(theAppPath) {
    }

    BaseApp::~BaseApp() {
    }

    void BaseApp::setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, int theScreenWidth, int theScreenHeight) {        
        ComponentMapInitializer::init();
        

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
        AssetProviderSingleton::get().ap()->addIncludePath("core/shaders/");
        AssetProviderSingleton::get().ap()->addIncludePath(appPath_ + "/textures");
        AssetProviderSingleton::get().ap()->addIncludePath(appPath_ + "/layouts");
        AssetProviderSingleton::get().ap()->addIncludePath(appPath_ + "/shaders");
        AssetProviderSingleton::get().ap()->addIncludePath(appPath_ + "/models");
        AssetProviderSingleton::get().ap()->addIncludePath(appPath_);
            
        std::string myOrientation = "free";         
    }
    
    void BaseApp::realize() {
        RealizeComponentVisitor myVisitor;
        visitComponents(myVisitor, _mySparkWindow);   
    }

    std::string
    BaseApp::findBestMatchedLayout(std::string theBaseName, int theScreenWidth, int theScreenHeight, std::string & theOrientation) {
        std::vector<std::string> myFiles = AssetProviderSingleton::get().ap()->getFilesFromPath(theBaseName);   
        int myScreensLargerSide = theScreenWidth > theScreenHeight ? theScreenWidth : theScreenHeight;
        int myScreensSmallerSide = myScreensLargerSide ==  theScreenHeight ? theScreenWidth : theScreenHeight;
        std::string myBestMatch = "";
        std::string myBestOrientation = "";
        std::string myBestLayoutName = "";
        int myBestLayoutWidth = 0;
        int myBestLayoutHeight = 0;
        
        bool myExactMatchFlag = false;
        int myLayoutWidth = 0;
        int myLayoutHeight = 0;
        std::string myOrientation = "";
        std::string myLayoutName = "";
        for (unsigned int i = 0; i < myFiles.size(); i++) {            
            if (getExtension(myFiles[i]) == "spark") {
                std::string myChoice = getDirectoryPart(theBaseName) + getFilenamePart(myFiles[i]);
                std::string myLayout = AssetProviderSingleton::get().ap()->getStringFromFile(myChoice);        
                xmlDocPtr doc = loadXMLFromMemory(myLayout);
                xmlNode* myRootNode = xmlDocGetRootElement(doc);
                XMLNodePtr myNode(new XMLNode(myRootNode));
                xmlFreeDoc(doc);
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
                        if (it->first == "orientation") {     
                            myOrientation = it->second;   
                        }
                    }
                }
                if (i == 0) {
                    myBestMatch = myChoice;
                    myBestOrientation = myOrientation;
                    myBestLayoutName = myLayoutName;
                    myBestLayoutWidth = myLayoutWidth;
                    myBestLayoutHeight = myLayoutHeight;
                }
                int myLayoutsLargerSide = myLayoutWidth > myLayoutHeight ? myLayoutWidth : myLayoutHeight;
                int myLayoutsSmallerSide = myScreensLargerSide ==  myLayoutHeight ? myLayoutWidth : myLayoutHeight;
                //AC_PRINT << "check layout '" << myLayoutName << " width " << myLayoutWidth << "'" << " height " << myLayoutHeight << " screen :" << theScreenWidth << " x" << theScreenHeight;
                if (myLayoutsLargerSide == myScreensLargerSide && myLayoutsSmallerSide == myScreensSmallerSide ) {
                    myBestMatch = myChoice;
                    myBestOrientation = myOrientation;
                    myBestLayoutName = myLayoutName;
                    myBestLayoutWidth = myLayoutWidth;
                    myBestLayoutHeight = myLayoutHeight;
                    myExactMatchFlag = true;
                    AC_PRINT << "Excellent we have a layout<->device match -> use layout '" << myBestLayoutName << "' file: '" << myBestMatch << "'";
                    AC_PRINT << "Layout : "<< myBestLayoutWidth << "x" <<  myBestLayoutHeight << " with orientation: '" <<  myBestOrientation << "'";
                    break;
                }
            }            
        }
        if (!myExactMatchFlag) {
            AC_PRINT << "Sorry, we did not find layout<->device match -> use default layout '" << myBestLayoutName << "' file: '" << myBestMatch << "'";
            AC_PRINT << "Layout : "<< myBestLayoutWidth << "x" <<  myBestLayoutHeight << " with orientation: '" <<  myBestOrientation << "'";
        }
        theOrientation = myBestOrientation;
        return myBestMatch;
    }
    
    void
    BaseApp::loadLayoutAndRegisterEvents(const std::string & theLayoutFile) {
        //load layout
        _mySparkWindow = boost::static_pointer_cast<spark::Window>(SparkComponentFactory::get().loadSparkComponentsFromFile(BaseAppPtr(this), theLayoutFile));

        //register for events
        spark::EventCallbackPtr myFrameCB = EventCallbackPtr(new MemberFunctionEventCallback<BaseApp, BaseAppPtr > ( shared_from_this(), &BaseApp::onFrame));
        _mySparkWindow->addEventListener(StageEvent::FRAME, myFrameCB);
        spark::EventCallbackPtr myCB = EventCallbackPtr(new MemberFunctionEventCallback<Window, WindowPtr>( _mySparkWindow, &Window::onTouch));
        _mySparkWindow->addEventListener(TouchEvent::TAP, myCB);
        spark::EventCallbackPtr myOnPauseCB = EventCallbackPtr(new MemberFunctionEventCallback<BaseApp, BaseAppPtr > ( shared_from_this(), &BaseApp::onPause));
        _mySparkWindow->addEventListener(StageEvent::PAUSE, myOnPauseCB);
    }
    
    void BaseApp::onEvent(std::string theEventString) {
        //AC_PRINT << "a string event came in :" << theEventString;
        EventPtr myEvent = spark::EventFactory::get().handleEvent(theEventString);
        myEvent->connect(_mySparkWindow);
        (*myEvent)();
        //AC_PRINT << "ate Event";
    }
    
    void BaseApp::onPause(EventPtr theEvent) {
        if (_mySparkWindow) {
            OnPauseComponentVisitor myVisitor;
            visitComponents(myVisitor, _mySparkWindow);   
        }
    }

    void BaseApp::onResume() {
        AC_PRINT << "onResume";
        if (_mySparkWindow) {
            OnResumeComponentVisitor myVisitor;
            visitComponents(myVisitor, _mySparkWindow);
        }
    }
    
    void BaseApp::onFrame(EventPtr theEvent) {
        AC_TRACE << "onFrame";
        StageEventPtr myEvent = boost::static_pointer_cast<StageEvent>(theEvent);
        animation::AnimationManager::get().doFrame(myEvent->getCurrentTime());
        _mySparkWindow->render();
        AC_TRACE << "onFrame done, currentTime "<< myEvent->getCurrentTime();
    }    
}

