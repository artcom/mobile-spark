#include "Window.h"

#include <algorithm>
#include <masl/XMLNode.h>
#include <masl/Logger.h>
#include <masl/MobileSDK.h>
#include <mar/Canvas.h>

#include "SparkComponentFactory.h"
#include "View.h"
#include "Visitors.h"

using namespace mar;
using namespace std;

namespace spark {

    const char * const Orientation::PORTRAIT = "portrait";
    const char * const Orientation::LANDSCAPE = "landscape";
    const char * const Orientation::FLOATING = "floating";
    const char * const Window::SPARK_TYPE = "Window";

    Window::Window(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        Widget(theApp, theXMLNode),
        // if we are running fullscreen, wait for the first onSize to setup viewport, otherwise use spark values
        _myWidth(_myXMLNode->getAttributeAs<float>("width",100)),
        _myHeight(_myXMLNode->getAttributeAs<float>("height",100)),
        _myFullScreenFlag(_myXMLNode->getAttributeAs<bool>("fullscreen", false)),
        _myClearColor(_myXMLNode->getAttributeAs<vector4>("clearColor", vector4(1,1,1,1))),
        _myOrientation(_myXMLNode->getAttributeAs<string>("orientation",""))
    {
        _myGLCanvas = CanvasPtr( new Canvas());
        _myGLCanvas->initGLState();
    }

    Window::~Window() {
    }

    void
    Window::realize() {
        Widget::realize();
        WindowPtr ptr = boost::static_pointer_cast<Window>(shared_from_this());
            
        EventCallbackPtr mySizeChangedCB = EventCallbackPtr(new WindowCB(ptr, &Window::onSizeChanged));
        addEventListener(WindowEvent::ON_RESIZE, mySizeChangedCB);
        if (_myOrientation != "") {
            masl::MobileSDK_Singleton::get().getNative()->freezeMobileOrientation(_myOrientation);            
        }
        
        VectorOfComponentPtr myViews = getChildrenByType(View::SPARK_TYPE);
        for (std::vector<ComponentPtr>::const_iterator it = myViews.begin(); it != myViews.end(); ++it) {
            ViewPtr myView = boost::static_pointer_cast<spark::View>(*it);
            _myUnrealizedWorlds.push_back(getChildByName(myView->getWorldName()));
        }
        AC_INFO << "worlds realize _myUnrealizedWorlds : "<< _myUnrealizedWorlds.size();        
    }

    vector2
    Window::getSize() const {
        int myScreensLargerSide = _myWidth > _myHeight ? _myWidth : _myHeight;
        int myScreensSmallerSide = myScreensLargerSide ==  _myHeight ? _myWidth : _myHeight;

        if (_myOrientation == Orientation::PORTRAIT) {
            return vector2(myScreensSmallerSide, myScreensLargerSide);
        } else if (_myOrientation == Orientation::LANDSCAPE) {
            return vector2(myScreensLargerSide, myScreensSmallerSide);
        }
        // floating
        return vector2(_myWidth, _myHeight);
    }

    void
    Window::onTouch(EventPtr theEvent) {
        TouchEventPtr myPickEvent = boost::static_pointer_cast<TouchEvent>(theEvent);
        AC_INFO << "Window::onTouch " << myPickEvent->getType() << " x: "<< myPickEvent->getX() << " y: " << myPickEvent->getY();
        ComponentPtr myPicked = pick2DAABBStyle(myPickEvent->getX(), myPickEvent->getY());
        if (myPicked) {
            AC_INFO << "____________picked " << *myPicked;
            EventPtr myEvent = EventPtr(new TouchEvent(TouchEvent::PICKED, myPicked, myPickEvent->getX(), myPickEvent->getY()));
            (*myEvent)();
        } else {
            AC_DEBUG << "nothing picked";
        }
    }

    void
    Window::onResume() {
        _myGLCanvas->initGLState();
        if (_myOrientation != "") {
            masl::MobileSDK_Singleton::get().getNative()->freezeMobileOrientation(_myOrientation);            
        }
    }

    void
    Window::onSizeChanged(EventPtr theEvent) {
        WindowEventPtr myEvent = boost::static_pointer_cast<WindowEvent>(theEvent);
        _myWidth = myEvent->size_[0];
        _myHeight= myEvent->size_[1];
    }
    void
    Window::render(){
        _myGLCanvas->preRender(getClearColor());
        // get all views
        VectorOfComponentPtr myViews = getChildrenByType(View::SPARK_TYPE);
        for (std::vector<ComponentPtr>::const_iterator it = myViews.begin(); it != myViews.end(); ++it) {
            ViewPtr myView = boost::static_pointer_cast<spark::View>(*it);
            ComponentPtr myWorld = getChildByName(myView->getWorldName());
            std::vector<ComponentPtr>::iterator i = find(_myUnrealizedWorlds.begin(), _myUnrealizedWorlds.end(), myWorld);
            
            if (i != _myUnrealizedWorlds.end()) {
                myWorld->realizeASync();
                if (myWorld->isAllRealized()) {
                    WindowPtr ptr = boost::static_pointer_cast<Window>(shared_from_this());                    
                    EventPtr myEvent = EventPtr(new WindowEvent(WindowEvent::WORLD_REALIZED, ptr, myWorld->getName()));
                    (*myEvent)();                    
                    _myUnrealizedWorlds.erase(i);    
                }
            } else {
                if (myView->isVisible()) {
                    AC_TRACE << "render with : " << _myWidth << "/" << _myHeight;
                    myView->activate(_myWidth, _myHeight);
                    // find world and render it
                    myView->renderWorld(myWorld);
                }
            }
        }
    }




    //////////////picking
    ComponentPtr
    Window::pick2DAABBStyle(const unsigned int x, const unsigned int y) {
        AC_DEBUG << "pick at " << x << ", " << y;
        VectorOfComponentPtr myViews = getChildrenByType(View::SPARK_TYPE);
        std::vector<std::pair<ComponentPtr, float> > myPickedComponentList;  //pairs of components and z
        //pick through worlds of all views
        for (std::vector<ComponentPtr>::const_iterator it = myViews.begin(); it != myViews.end(); ++it) {
            ViewPtr myView = boost::static_pointer_cast<spark::View>(*it);
            //use relative touch point
            CollectAABBComponentVisitor myVisitor(myPickedComponentList, (float)x/(float)_myWidth, (float)y/(float)_myHeight,
                                                  myView->getCamera()->getProjectionMatrix());
            parentFirstVisitComponents(myVisitor, getChildByName(myView->getWorldName()));
        }
        AC_DEBUG << "collected " << myPickedComponentList.size() << " components";
        if (myPickedComponentList.size() > 0) {
            sort(myPickedComponentList.begin(), myPickedComponentList.end(), sortByZ);
            return myPickedComponentList.begin()->first;
        }
        ComponentPtr null;
        return null;
    }

    bool
    sortByZ(std::pair<ComponentPtr, float> i, std::pair<ComponentPtr, float> j) {
        return (i.second > j.second);
    }

}
