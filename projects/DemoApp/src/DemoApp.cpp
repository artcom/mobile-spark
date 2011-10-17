#include "DemoApp.h"

#include <cstdlib>

#include <masl/AssetProvider.h>
#include <masl/Callback.h>
#include <masl/Logger.h>
#include <masl/MobileSDK.h>
#include <masl/file_functions.h>

#include <animation/AnimationManager.h>
#include <animation/ParallelAnimation.h>
#include <animation/SequenceAnimation.h>
#include <animation/DelayAnimation.h>
#include <animation/Easing.h>

#include <spark/Window.h>
#include <spark/Event.h>
#include <spark/Rectangle.h>
#include <spark/Transform.h>
#include <spark/Shape3D.h>
#include <spark/Text.h>
#include <spark/SparkComponentFactory.h>
#include <spark/AppProvider.h>

#include "DemoAppComponentMapInitializer.h"

//static bool ourVibratorFlag = false;
using namespace spark;
using namespace masl;

/////////////////////////////////////////////////////////////////////////App-Instance
#ifdef ANDROID
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    spark::AppProvider::get().setApp(masl::Ptr<demoapp::DemoApp>(new demoapp::DemoApp()));
    return JNI_VERSION_1_6;
}
#endif


/////////////////// Application code, this should be in java or script language later...
namespace demoapp {

    WidgetPropertyAnimationPtr myAmazoneRotation;

    DemoApp::DemoApp():BaseApp("DemoApp"), _myCurrentSlide(0) {
    }

    DemoApp::~DemoApp() {
    }

    void freeFunction() {
        AC_PRINT << "hello from free function";
    }
    void freeFunctionEventCB(EventPtr theEvent) {
        AC_PRINT << "hello from free function event callback for eventtype " << theEvent->getType();
    }

    void DemoApp::setup(const masl::UInt64 theCurrentMillis, const std::string & theAssetPath, int theScreenWidth, int theScreenHeight) {

        BaseApp::setup(theCurrentMillis, theAssetPath, theScreenWidth, theScreenHeight);
        DemoAppComponentMapInitializer::init();

        loadLayoutAndRegisterEvents("/main", theScreenWidth, theScreenHeight);
        
        AC_PRINT<<"AC_LOG_VERBOSITY env: "<<getenv("AC_LOG_VERBOSITY");

        DemoAppPtr ptr = boost::static_pointer_cast<DemoApp>(shared_from_this());
        EventCallbackPtr mySizeChangedCB = EventCallbackPtr(new DemoEventCB(ptr, &DemoApp::onSizeChanged));
        _mySparkWindow->addEventListener(WindowEvent::ON_RESIZE, mySizeChangedCB);

        ComponentPtr my2DWorld = _mySparkWindow->getChildByName("2dworld");

        //test free function on touch
        spark::EventCallbackPtr myFreeCB = EventCallbackPtr(new FreeFunctionEventCallback(freeFunctionEventCB));
        _mySparkWindow->addEventListener(TouchEvent::TAP, myFreeCB);

        //button callbacks
        spark::EventCallbackPtr myPickedCB = EventCallbackPtr(new DemoEventCB(ptr, &DemoApp::onControlButton));
        ComponentPtr myBackButton = my2DWorld->getChildByName("backbutton", true);
        ComponentPtr myNextButton = my2DWorld->getChildByName("nextbutton", true);
        myBackButton->addEventListener(TouchEvent::PICKED, myPickedCB);
        myNextButton->addEventListener(TouchEvent::PICKED, myPickedCB);
        
        spark::EventCallbackPtr myCreationCB = EventCallbackPtr(new DemoEventCB(ptr, &DemoApp::onCreationButton));
        ComponentPtr myCreationButton = my2DWorld->getChildByName("creationbutton", true);
        myCreationButton->addEventListener(TouchEvent::PICKED, myCreationCB);

        spark::EventCallbackPtr mySwitchLanguageCB = EventCallbackPtr(new DemoEventCB(ptr, &DemoApp::onLanguageSwitch));
        spark::ComponentPtr myLanguageButton = _mySparkWindow->getChildByName("languagebutton", true);
        myLanguageButton->addEventListener(TouchEvent::PICKED, mySwitchLanguageCB);

        spark::EventCallbackPtr myLoadSceneCB = EventCallbackPtr(new DemoEventCB(ptr, &DemoApp::onLoadScene));
        spark::ComponentPtr myLoadButton = _mySparkWindow->getChildByName("load_button", true);
        myLoadButton->addEventListener(TouchEvent::PICKED, myLoadSceneCB);

		//touch gestures
        spark::EventCallbackPtr myAnimationCB = EventCallbackPtr(new DemoEventCB(ptr, &DemoApp::onTouch));
        _mySparkWindow->addEventListener(TouchEvent::TAP, myAnimationCB);

		//test swipe gestures
        spark::EventCallbackPtr mySwipeCB = EventCallbackPtr(new DemoEventCB(ptr,&DemoApp::onSwipeGesture));
        _mySparkWindow->addEventListener(GestureEvent::SWIPE_LEFT, mySwipeCB);
		_mySparkWindow->addEventListener(GestureEvent::SWIPE_RIGHT, mySwipeCB);

		//test pinch gestures
        spark::EventCallbackPtr myPinchCB = EventCallbackPtr(new DemoEventCB(ptr,&DemoApp::onPinchGesture));
        _mySparkWindow->addEventListener(GestureEvent::PINCH, myPinchCB);
        
        //test sensors
        spark::EventCallbackPtr mySensorCB = EventCallbackPtr(new DemoEventCB(ptr,&DemoApp::onSensorEvent));
        _mySparkWindow->addEventListener(SensorEvent::ORIENTATION, mySensorCB);
        spark::EventCallbackPtr mySensorLightCB = EventCallbackPtr(new DemoEventCB(ptr,&DemoApp::onSensorLightEvent));
        _mySparkWindow->addEventListener(SensorEvent::LIGHT, mySensorLightCB);
        spark::EventCallbackPtr mySensorGyroCB = EventCallbackPtr(new DemoEventCB(ptr,&DemoApp::onSensorGyroEvent));
        _mySparkWindow->addEventListener(SensorEvent::GYROSCOPE, mySensorGyroCB);

        //http-request stuff
        _myErrorMessage = boost::static_pointer_cast<spark::Text>(_mySparkWindow->getChildByName("curl_error", true));
        _myRequestManager.setOnErrorCallback(
            masl::RequestCallbackPtr(new DemoRequestCB(ptr, &DemoApp::onErrorRequestCB)));
        _myRequestManager.getRequest("http://www.einsfeld.de/mobile-spark/string.txt",
            masl::RequestCallbackPtr(new DemoRequestCB(ptr, &DemoApp::onTextRequestReady)));
        _myRequestManager.getRequest("http://www.einsfeld.de/mobile-spark/",
            masl::RequestCallbackPtr(new DemoRequestCB(ptr, &DemoApp::onGetRequestReady)));
        _myRequestManager.postRequest("http://www.einsfeld.de/mobile-spark/", "id=23&value=post data",
            masl::RequestCallbackPtr(new DemoRequestCB(ptr, &DemoApp::onPostRequestReady)));
        //data is not visible at server, not sure how this is intended to work in Request.cpp
        _myRequestManager.putRequest("http://www.einsfeld.de/mobile-spark/", "hello world from put in DemoApp",
            masl::RequestCallbackPtr(new DemoRequestCB(ptr, &DemoApp::onPutRequestReady)));
        _myRequestManager.deleteRequest("http://www.einsfeld.de/mobile-spark/",
            masl::RequestCallbackPtr(new DemoRequestCB(ptr, &DemoApp::onDeleteRequestReady)));
        animation::DelayAnimationPtr myRepeatingDateRequest = animation::DelayAnimationPtr(new animation::DelayAnimation(1000));
        myRepeatingDateRequest->setLoop(true);
        myRepeatingDateRequest->setOnFinish(masl::CallbackPtr(new DemoCB(ptr, &DemoApp::onRepeatingDateRequest)));
        animation::AnimationManager::get().play(myRepeatingDateRequest);

        //animation of amazone
        WidgetPropertyAnimationPtr myXRotate, myYRotate, myZRotate;
        ComponentPtr myComponent = _mySparkWindow->getChildByName("3dworld")->getChildByName("transform")->getChildByName("theAmazone");
        WidgetWeakPtr myShape = WidgetWeakPtr(WidgetPtr(boost::static_pointer_cast<spark::Shape3D>(myComponent)));
        myAmazoneRotation = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myShape, &Widget::setRotationY, 0, M_PI * 2, 90000));
        myAmazoneRotation->setLoop(true);
        animation::AnimationManager::get().play(myAmazoneRotation);

        //animation of 3d object
        myComponent = my2DWorld->getChildByName("funnyShape", true);
        myShape = WidgetWeakPtr(WidgetPtr(boost::static_pointer_cast<spark::Shape3D>(myComponent)));
        myXRotate = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myShape, &Widget::setRotationX, 0, 6.28, 7000));
        myXRotate->setLoop(true);
        myYRotate = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myShape, &Widget::setRotationY, 0, 6.28, 9000));
        myYRotate->setLoop(true);
        myZRotate = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myShape, &Widget::setRotationZ, 0, 6.28, 13000));
        myZRotate->setLoop(true);
        animation::AnimationManager::get().play(myXRotate);
        animation::AnimationManager::get().play(myYRotate);
        animation::AnimationManager::get().play(myZRotate);

        //add looping sequence animation for rectangle
        myComponent = my2DWorld->getChildByName("AnimatedRectangleSlide")->getChildByName("transform")->getChildByName("redObject");
        WidgetWeakPtr myRectangle = WidgetWeakPtr(WidgetPtr(boost::static_pointer_cast<spark::Rectangle>(myComponent)));
        WidgetPropertyAnimationPtr myAnimation1 = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myRectangle, &Widget::setScaleY, 0.7, 8, 500));
        WidgetPropertyAnimationPtr myAnimation2 = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(myRectangle, &Widget::setScaleY, 8, 0.7, 1500, animation::EasingFnc(animation::easeInOutQuint)));
        WidgetPropertyAnimationPtr myAnimation3 = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(myRectangle, &Widget::setAlpha, 1, 0.2, 1500, animation::EasingFnc(animation::linearTween)));
        WidgetPropertyAnimationPtr myAnimation4 = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(myRectangle, &Widget::setAlpha, 0.2, 1, 1500, animation::EasingFnc(animation::linearTween)));
        animation::DelayAnimationPtr myDelay = animation::DelayAnimationPtr(new animation::DelayAnimation(2000));
        animation::SequenceAnimationPtr mySequence = animation::SequenceAnimationPtr(new animation::SequenceAnimation());
        animation::ParallelAnimationPtr myParallel1 = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
        animation::ParallelAnimationPtr myParallel2 = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
        myParallel1->add(myAnimation1);
        myParallel1->add(myAnimation3);
        myParallel2->add(myAnimation2);
        myParallel2->add(myAnimation4);
        mySequence->add(myParallel1);
        mySequence->add(myParallel2);
        mySequence->add(myDelay);
        mySequence->setLoop(true);
        //mySequence->setOnFinish(masl::CallbackPtr(new masl::FreeFunctionCallback(freeFunction)));
        animation::AnimationManager::get().play(mySequence);

        //collect slides and views
        ContainerPtr myContainer = boost::static_pointer_cast<spark::Container>(my2DWorld);
        const VectorOfComponentPtr & myChildren = myContainer->getChildrenByType("SlideImpl");
        for (size_t i = 0; i < myChildren.size(); i++) {
            SlideImplPtr mySlide = boost::static_pointer_cast<SlideImpl>(myChildren[i]);
            if (mySlide) {
                _mySlides.push_back(mySlide);
                mySlide->setVisible(false);
                mySlide->setSensible(false);
            }
            AC_DEBUG << "add slide to slides : " << mySlide->getName();
        }
        _myCurrentSlide = 0;
        _mySlides[_myCurrentSlide]->setVisible(true);
        _mySlides[_myCurrentSlide]->setSensible(true);

        AC_DEBUG << "found #" << _mySlides.size() << " slides";        
    }

    void DemoApp::onErrorRequestCB(masl::RequestPtr theRequest) {
        if (!_myLoadingMessage) {
            _myLoadingMessage = boost::static_pointer_cast<spark::Text>(_mySparkWindow->getChildByName("loading", true));
        }
        _myLoadingMessage->setVisible(false);
        if (_myErrorExpiredDelay) {
            _myErrorExpiredDelay->cancel();
        }
        AC_DEBUG << "error with request for URL " << theRequest->getURL();
        DemoAppPtr ptr = boost::static_pointer_cast<DemoApp>(shared_from_this());    	
        _myErrorMessage->setVisible(true);
        _myErrorMessage->setText("error requesting " + theRequest->getURL());
        _myErrorExpiredDelay = animation::DelayAnimationPtr(new animation::DelayAnimation(10000));
        _myErrorExpiredDelay->setOnFinish(masl::CallbackPtr(new DemoCB(ptr, &DemoApp::onErrorExpired)));
        animation::AnimationManager::get().play(_myErrorExpiredDelay);
    }
    void DemoApp::onErrorExpired() {
        _myErrorMessage->setVisible(false);
    }
    void DemoApp::onRepeatingDateRequest() {
        DemoAppPtr ptr = boost::static_pointer_cast<DemoApp>(shared_from_this());    	
        _myRequestManager.getRequest("http://www.einsfeld.de/mobile-spark/currentDate.php",
            masl::RequestCallbackPtr(new DemoRequestCB(ptr, &DemoApp::onDateRequestReady)));
    }
    void DemoApp::onTextRequestReady(RequestPtr theRequest) {
        TextPtr myText = boost::static_pointer_cast<spark::Text>(_mySparkWindow->getChildByName("textfromserver", true));
        myText->setText(theRequest->getResponseString());
    }
    void DemoApp::onGetRequestReady(RequestPtr theRequest) {
        TextPtr myText = boost::static_pointer_cast<spark::Text>(_mySparkWindow->getChildByName("getrequest", true));
        myText->setText(theRequest->getResponseString());
    }
    void DemoApp::onPostRequestReady(RequestPtr theRequest) {
        TextPtr myText = boost::static_pointer_cast<spark::Text>(_mySparkWindow->getChildByName("postrequest", true));
        myText->setText(theRequest->getResponseString());
    }
    void DemoApp::onPutRequestReady(RequestPtr theRequest) {
        TextPtr myText = boost::static_pointer_cast<spark::Text>(_mySparkWindow->getChildByName("putrequest", true));
        myText->setText(theRequest->getResponseString());
    }
    void DemoApp::onDeleteRequestReady(RequestPtr theRequest) {
        TextPtr myText = boost::static_pointer_cast<spark::Text>(_mySparkWindow->getChildByName("deleterequest", true));
        myText->setText(theRequest->getResponseString());
    }
    void DemoApp::onDateRequestReady(RequestPtr theRequest) {
        TextPtr myText = boost::static_pointer_cast<spark::Text>(_mySparkWindow->getChildByName("datefromserver", true));
        myText->setText(theRequest->getResponseString());
    }
    void DemoApp::onSparkRequestReady(RequestPtr theRequest) {
        DemoAppPtr ptr = boost::static_pointer_cast<DemoApp>(shared_from_this());    	
        std::string myNewSpark = theRequest->getResponseString();
        std::vector<std::string> assetList = spark::SparkComponentFactory::get().createSrcListFromSpark(myNewSpark);
        masl::AssetProviderSingleton::get().ap()->storeInFile("downloads/scene.spark", myNewSpark);
        _myRequestManager.getAllRequest("http://www.einsfeld.de/mobile-spark/assets/", assetList,
            masl::RequestCallbackPtr(new DemoRequestCB(ptr, &DemoApp::onAssetRequestReady)),
            masl::RequestCallbackPtr(new DemoRequestCB(ptr, &DemoApp::onAllAssetsRequestReady)),"/downloads/");
        AC_DEBUG << "headers of spark-request";
        std::multimap<std::string, std::string> headers = theRequest->getResponseHeaders();
        for (std::multimap<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it) {
            AC_DEBUG << "http headers " << it->first << ": " << it->second; 
        }
        AC_DEBUG << "last modified: " << theRequest->getResponseHeader("Last-Modified");
    }
    void DemoApp::onAssetRequestReady(masl::RequestPtr theRequest) {
        AC_DEBUG << "on Asset ready, request url was " << theRequest->getURL();
        std::vector<char> myBlock = theRequest->getResponseBinary();
        masl::AssetProviderSingleton::get().ap()->storeInFile("downloads/" + masl::getFilenamePart(theRequest->getURL()), myBlock);
        AC_DEBUG << "headers of png-request";
        std::multimap<std::string, std::string> headers = theRequest->getResponseHeaders();
        for (std::multimap<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it) {
            AC_DEBUG << "http headers " << it->first << ": " << it->second; 
        }
        AC_DEBUG << "last modified: " << theRequest->getResponseHeader("Last-Modified");
    }
    void DemoApp::onAllAssetsRequestReady(masl::RequestPtr theRequest) {
        AC_DEBUG << "on AllAsset Ready";
        _myLoadingMessage->setVisible(false);
        _myLoadingAnimation->cancel();
        DemoAppPtr ptr = boost::static_pointer_cast<DemoApp>(shared_from_this());    	
        spark::TransformPtr myTransform = boost::static_pointer_cast<spark::Transform>(_mySparkWindow->getChildByName("InternetSlide", true));
        ComponentPtr myNewSpark = spark::SparkComponentFactory::get().loadSparkComponentsFromFile(ptr, "/downloads/scene.spark");
        myTransform->addChild(myNewSpark);
    }

    void DemoApp::onControlButton(EventPtr theEvent) {
        AC_DEBUG << "on control button";
        //ourVibratorFlag = true;
        MobileSDK_Singleton::get().getNative()->vibrate(10);        
    	changeSlide(theEvent->getTarget()->getName() == "backbutton" ? -1 :  +1);    
    }
    
    void DemoApp::onStartSlideSwipe() {
        AC_PRINT << "set : " << _mySlides[_myNextSlide]->getName() << " to visible";
        _mySlides[_myNextSlide]->setVisible(true);
        _mySlides[_myNextSlide]->setX(_mySparkWindow->getSize()[0]);        
    }
    void DemoApp::onFinishSlideSwipe() {
    	//turn amazone animation on when leaving 3D-Viewer-Slide:
    	if (_mySlides[_myCurrentSlide]->getName() == "3D-Viewer-Slide") animation::AnimationManager::get().play(myAmazoneRotation);
        _mySlides[_myCurrentSlide]->setVisible(false);
        _mySlides[_myCurrentSlide]->setSensible(false);
        AC_PRINT << "set : " << _mySlides[_myCurrentSlide]->getName() << " to invisible";
        _myCurrentSlide = _myNextSlide;
        _mySlides[_myCurrentSlide]->setSensible(true);
        if (_mySlides[_myCurrentSlide]->getName() == "3D-Viewer-Slide") myAmazoneRotation->cancel();
        

    }
    
    void DemoApp::onSwipeGesture(EventPtr theEvent) {
    	AC_DEBUG << "on Swipe Gesture";
    	changeSlide(theEvent->getType() == "swipe-right" ? -1 :  +1);
    }
    
    void DemoApp::onFrame(EventPtr theEvent) {
        BaseApp::onFrame(theEvent);            
        _myRequestManager.handleRequests();
    }
    
    void DemoApp::changeSlide(int theDirection) {
        AC_INFO << "change slide: " << theDirection << " next slide:" << _myNextSlide << " currentslide: " << _myCurrentSlide;
        DemoAppPtr ptr = boost::static_pointer_cast<DemoApp>(shared_from_this());    	
        animation::ParallelAnimationPtr mySequence = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
        mySequence->setOnPlay(masl::CallbackPtr(new DemoCB(ptr, &DemoApp::onStartSlideSwipe)));
        WidgetPropertyAnimationPtr mySwipeOut = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(
                                                WidgetWeakPtr(WidgetPtr(_mySlides[_myCurrentSlide])),
                                                &Widget::setX, 0, _mySparkWindow->getSize()[0] * theDirection * -1, 500,
                                                animation::EasingFnc(animation::easeInOutQuad)));
        _myNextSlide = (_myCurrentSlide + _mySlides.size() + theDirection) % _mySlides.size();
        WidgetPropertyAnimationPtr mySwipeIn = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(
                                               WidgetWeakPtr(WidgetPtr(_mySlides[_myNextSlide])),
                                               &Widget::setX, _mySparkWindow->getSize()[0] * theDirection, 0, 500,
                                               animation::EasingFnc(animation::easeInOutQuad)));
        mySequence->setOnFinish(masl::CallbackPtr(new DemoCB(ptr, &DemoApp::onFinishSlideSwipe)));
    	mySequence->add(mySwipeOut);
    	mySequence->add(mySwipeIn);
        animation::AnimationManager::get().play(mySequence);
    }
    
    void DemoApp::onPinchGesture(EventPtr theEvent) {
        if (_mySlides[_myCurrentSlide]->getName() =="3D-Viewer-Slide") {
        	AC_DEBUG << "on Pinch Gesture";
        	GestureEventPtr myEvent = boost::static_pointer_cast<GestureEvent>(theEvent);
        	float myScaleFactor = myEvent->getFactor(); 
        	ComponentPtr my3dView = _mySparkWindow->getChildByName("3dworld")->getChildByName("transform")->getChildByName("theAmazone");
            boost::static_pointer_cast<Shape3D>(my3dView)->setScaleX(myScaleFactor);
    		boost::static_pointer_cast<Shape3D>(my3dView)->setScaleY(myScaleFactor);
    		boost::static_pointer_cast<Shape3D>(my3dView)->setScaleZ(myScaleFactor);

		}
    }
    

	void DemoApp::onSensorEvent(EventPtr theEvent) {
		if (_mySlides[_myCurrentSlide]->getName() != "3D-Viewer-Slide") return;
    	SensorEventPtr myEvent = boost::static_pointer_cast<SensorEvent>(theEvent);
    	float myX = myEvent->getValue0()*3.1417/180; 
    	float myY = myEvent->getValue1()*3.1417/180; 
    	//float myZ = myEvent->getValue2()*3.1417/180;
    	ComponentPtr myAmazoneObject = _mySparkWindow->getChildByName("3dworld")->getChildByName("transform")->getChildByName("theAmazone");
        Shape3DPtr myAmazone = boost::static_pointer_cast<spark::Shape3D>(myAmazoneObject);
		myAmazone -> setRotationX(-myY - 1.57);
		myAmazone -> setRotationY(-myX * 2); // x2 for more effect
    }

	void DemoApp::onSensorLightEvent(EventPtr theEvent) {
    	SensorEventPtr myEvent = boost::static_pointer_cast<SensorEvent>(theEvent);
    	float myLight = myEvent->getValue0();
       	ComponentPtr myLightObject = _mySparkWindow->getChildByName("transformLight", true);
        TransformPtr myLightRectangle = boost::static_pointer_cast<spark::Transform>(myLightObject);
		myLightRectangle->setY(myLight / 10);
    }
    
    void DemoApp::onSensorGyroEvent(EventPtr theEvent) {
    	SensorEventPtr myEvent = boost::static_pointer_cast<SensorEvent>(theEvent);
    	float myX = myEvent->getValue0();
       	float myY = myEvent->getValue1();
       	ComponentPtr myGyroObject = _mySparkWindow->getChildByName("gyro", true);
        RectanglePtr myGyroRectangle = boost::static_pointer_cast<spark::Rectangle>(myGyroObject);
		myGyroRectangle->setX(myX*20-15);
		myGyroRectangle->setY(myY*20-15);
    }

    void DemoApp::onCreationButton(EventPtr theEvent) {
        AC_DEBUG << "on creation button";
        //ourVibratorFlag = true;
        MobileSDK_Singleton::get().getNative()->vibrate(10);                
        ComponentPtr myTransform = _mySparkWindow->getChildByName("2dworld")->getChildByName("ObjectCreationSlide");
        ComponentPtr myCreated = myTransform->getChildByName("created_from_code");
        if (myCreated) {
            boost::static_pointer_cast<spark::Container>(myTransform)->removeChild(myCreated);
        } else {
            //TRICK: this has to be done on main thread -> use delay animation with 0 duration achieve this
            animation::DelayAnimationPtr myDelay = animation::DelayAnimationPtr(new animation::DelayAnimation(0));
            DemoAppPtr ptr = boost::static_pointer_cast<DemoApp>(shared_from_this());
            myDelay->setOnFinish(masl::CallbackPtr(new DemoCB(ptr, &DemoApp::insertCreatedComponent)));
            animation::AnimationManager::get().play(myDelay);
        }
    }

    void DemoApp::onLoadScene(EventPtr theEvent) {
        ComponentPtr myScene = _mySparkWindow->getChildByName("spark_from_remote_server", true);
        if (myScene) {
            boost::static_pointer_cast<spark::Container>(myScene->getParent().lock())->removeChild(myScene);
            myScene = ComponentPtr();
        } else {
            _myLoadingMessage = boost::static_pointer_cast<spark::Text>(_mySparkWindow->getChildByName("loading", true));
            _myLoadingMessage->setVisible(true);
            WidgetPropertyAnimationPtr myAnimation1 = WidgetPropertyAnimationPtr(
                    new WidgetPropertyAnimation(WidgetWeakPtr(WidgetPtr(_myLoadingMessage)), &Widget::setX, 30, 300, 500));
            WidgetPropertyAnimationPtr myAnimation2 = WidgetPropertyAnimationPtr(
                    new WidgetPropertyAnimation(WidgetWeakPtr(WidgetPtr(_myLoadingMessage)), &Widget::setX, 300, 30, 500));
            animation::SequenceAnimationPtr mySequence = animation::SequenceAnimationPtr(new animation::SequenceAnimation());
            mySequence->add(myAnimation1);
            mySequence->add(myAnimation2);
            mySequence->setLoop(true);
            animation::AnimationManager::get().play(mySequence);
            _myLoadingAnimation = mySequence;
            DemoAppPtr ptr = boost::static_pointer_cast<DemoApp>(shared_from_this());    	
            _myRequestManager.getRequest("http://www.einsfeld.de/mobile-spark/scene.spark",
                masl::RequestCallbackPtr(new DemoRequestCB(ptr, &DemoApp::onSparkRequestReady)),"/downloads/");
        }
    }

    void DemoApp::onLanguageSwitch(EventPtr theEvent) {
        LANGUAGE myLanguage = _mySparkWindow->getLanguage();
        _mySparkWindow->switchLanguage(myLanguage == spark::DE ? spark::EN : spark::DE);
    }

    void DemoApp::insertCreatedComponent() {
        ComponentPtr myTransform = _mySparkWindow->getChildByName("2dworld")->getChildByName("ObjectCreationSlide");
        ComponentPtr myCreated = SparkComponentFactory::get().loadSparkComponentsFromString(shared_from_this(), 
                "<Rectangle name=\"created_from_code\" width=\"300\" height=\"10\" color=\"[1.0,1.0,0.0]\"/>"); 
        ContainerPtr myContainer = boost::static_pointer_cast<spark::Container>(myTransform);
        myContainer->addChild(myCreated);

    }

    void DemoApp::onTouch(EventPtr theEvent) {
        TouchEventPtr myEvent = boost::static_pointer_cast<TouchEvent>(theEvent);
        AC_DEBUG << "on touch "<<myEvent->getType()<<" x:"<<myEvent->getX()<<" ,y:"<<myEvent->getY();
        //add parallel animations
        ComponentPtr mySlide = _mySparkWindow->getChildByName("2dworld")->getChildByName("AnimatedRectangleSlide");
        ComponentPtr myObject = mySlide->getChildByName("greenObject");
        WidgetWeakPtr myRectangle = WidgetWeakPtr(WidgetPtr(boost::static_pointer_cast<spark::Rectangle>(myObject)));
        WidgetPropertyAnimationPtr myAnimationX = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(myRectangle, &Widget::setX, myRectangle.lock()->getX(), myEvent->getX(), 1000,
                    animation::EasingFnc(animation::easeInOutElastic)));
        WidgetPropertyAnimationPtr myAnimationY = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(myRectangle, &Widget::setY, myRectangle.lock()->getY(), myEvent->getY(), 1000,
                    animation::EasingFnc(animation::easeInOutElastic)));
        myObject = mySlide->getChildByName("transform")->getChildByName("blueObject");
        myRectangle = WidgetWeakPtr(WidgetPtr(boost::static_pointer_cast<spark::Rectangle>(myObject)));
        WidgetPropertyAnimationPtr myRotationAnimation = WidgetPropertyAnimationPtr(new WidgetPropertyAnimation(myRectangle, &Widget::setRotationZ, 0, M_PI * 2, 5000));
        animation::ParallelAnimationPtr myParallel = animation::ParallelAnimationPtr(new animation::ParallelAnimation());
        myParallel->add(myAnimationX);
        myParallel->add(myAnimationY);
        myParallel->add(myRotationAnimation);
        animation::AnimationManager::get().play(myParallel);
    }
    
    void
    DemoApp::centerSlideTitlesToNewCanvasSize(int theWidth, int theHeight) {
        for (std::vector<SlideImplPtr>::iterator it = _mySlides.begin(); it != _mySlides.end(); ++it) {
            (*it)->centerTitles(theWidth, theHeight);
        }
    }
    
    void DemoApp::onSizeChanged(EventPtr theEvent) {
        WindowEventPtr myEvent = boost::static_pointer_cast<WindowEvent>(theEvent);            
        centerSlideTitlesToNewCanvasSize(myEvent->size_[0], myEvent->size_[1]);
    }
}


