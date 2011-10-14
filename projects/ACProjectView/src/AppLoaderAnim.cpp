#include "AppLoaderAnim.h"
#include <spark/SparkComponentFactory.h>
#include <spark/Window.h>

using namespace spark;

namespace acprojectview {

    const char * const AppLoaderAnim::SPARK_TYPE = "AppLoaderAnim";
    
        
    AppLoaderAnim::AppLoaderAnim(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
        : Transform(theApp, theXMLNode), _myLastTime(0), _myCurrentImageIndex(0) {
    }

    AppLoaderAnim::~AppLoaderAnim() {}

    void
    AppLoaderAnim::realize() {
        Transform::realize();        
        WindowPtr myWindow = boost::static_pointer_cast<spark::Window>(getRoot());
        AppLoaderAnimPtr ptr = boost::static_pointer_cast<AppLoaderAnim>(shared_from_this());
            
        _myFrameCB = EventCallbackPtr(new AppLoaderAnimCB(ptr, &AppLoaderAnim::onFrame));
        myWindow->addEventListener(StageEvent::FRAME, _myFrameCB);
            
        spark::EventCallbackPtr myCB = EventCallbackPtr(new AppLoaderAnimCB(ptr, &AppLoaderAnim::removeOnFrameListener));
        myWindow->addEventListener(WindowEvent::WORLD_REALIZED, myCB);

        const VectorOfComponentPtr & myChildren = getChildrenByType("Image");
        for (size_t i = 0; i < myChildren.size(); i++) {
            ImagePtr myImage = boost::static_pointer_cast<Image>(myChildren[i]);
            if (myImage) {
                _myImages.push_back(myImage);
                myImage->setVisible(false);
                myImage->setSensible(false);
            }
        }
        _myImages[_myCurrentImageIndex]->setVisible(true);
    }
    
    void
    AppLoaderAnim::removeOnFrameListener(EventPtr theEvent) {
        WindowEventPtr myEvent = boost::static_pointer_cast<WindowEvent>(theEvent);
        AC_PRINT << *myEvent;
        if (myEvent->worldname_ == "2dworld") {
            WindowPtr myWindow = boost::static_pointer_cast<spark::Window>(getRoot());
            myWindow->removeEventListener(StageEvent::FRAME, _myFrameCB);

        }
    }
    void
    AppLoaderAnim::onFrame(EventPtr theEvent) {
        StageEventPtr myEvent = boost::static_pointer_cast<StageEvent>(theEvent);
        
        double myDiff = myEvent->getCurrentTime() - _myLastTime;
        if (myDiff > float(1000)/_myImages.size()) {
            _myImages[_myCurrentImageIndex]->setVisible(false);
            _myCurrentImageIndex = (_myCurrentImageIndex+1)%_myImages.size();
            _myImages[_myCurrentImageIndex]->setVisible(true);
            
            _myLastTime = myEvent->getCurrentTime();
        }        
    }
}
