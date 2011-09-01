#include "Window.h"

#include <algorithm>
#include <masl/Logger.h>

#include "SparkComponentFactory.h"
#include "View.h"

using namespace mar;

namespace spark {

    //needed for component factory
    //namespace  {
        ComponentPtr createWindow(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent) {
            return WindowPtr(new Window(theApp, theXMLNode, theParent));
        };
        //const bool registered = spark::SparkComponentFactory::get().registerComponent("Window", spark::createWindow);
    //}


    Window::Window(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, 
                   ComponentPtr theParent):
        Container(theApp, theXMLNode, theParent), 
        _myWidth(0), _myHeight(0){
            
        _myFullScreenFlag = _myXMLNode->getBoolValue("fullscreen", false);
        _myClearColor = _myXMLNode->getVector4Value("clearColor", vector4(1,1,1,1));

        // if we are running fullscreen, wait for the first onSize to setup viewport, otherwise use spark values
        //if (!_myFullScreenFlag) {
            onSizeChanged(_myXMLNode->getFloatValue("width",100), _myXMLNode->getFloatValue("height",100));
        //}        
    }

    Window::~Window() {
    }
    
    void 
    Window::onTouch(EventPtr theEvent) { 
        TouchEventPtr myEvent = boost::static_pointer_cast<TouchEvent>(theEvent);
        AC_PRINT<<"hallo evt: "<< myEvent->getType() << " x: "<< myEvent->getX();

        //picking!!!
        ComponentPtr myPicked = pick2DBoxStyle(myEvent->getX(), myEvent->getY());
        if (myPicked) {
            AC_PRINT << "picked " << myPicked->getName();
        } else {
            AC_PRINT << "nothing picked";
        }
    }

    void 
    Window::onSizeChanged(int theWidth, int theHeight) {
        _myWidth = theWidth; 
        _myHeight = theHeight;
    }

    void 
    Window::render() const {        
        // get all views
        VectorOfComponentPtr myViews = getChildrenByType("View");
        for (std::vector<ComponentPtr>::const_iterator it = myViews.begin(); it != myViews.end(); ++it) {
            ViewPtr myView = boost::static_pointer_cast<spark::View>(*it);
            //AC_PRINT << "render with : " << _myWidth << "/" << _myHeight;
            myView->activate(_myWidth, _myHeight);
            // find world and render it
            myView->renderWorld(getChildByName(myView->getWorldName()));
        }        
    }

    //////////////picking
    
    ComponentPtr
    Window::pick2DBoxStyle(const unsigned int x, const unsigned int y) {
        AC_PRINT << "pick at " << x << ", " << y;
        std::vector<std::pair<ComponentPtr, float> > myPickedComponentList;  //pairs of components and z
        runThroughTreeAndCollectPickedComponents(myPickedComponentList);  //do it with vistor pattern?
        sort(myPickedComponentList.begin(), myPickedComponentList.end(), sortByZ);
        return myPickedComponentList.begin()->first;
    }

    void 
    Window::runThroughTreeAndCollectPickedComponents(std::vector<std::pair<ComponentPtr, float> > & theList) {
        //mock
        theList.push_back(std::pair<ComponentPtr, float>(shared_from_this(), 0));
    }

    bool 
    sortByZ(std::pair<ComponentPtr, float> i, std::pair<ComponentPtr, float> j) { 
        return (i.second < j.second); 
    }

}
