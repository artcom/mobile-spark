#include "Window.h"

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
        if (!_myFullScreenFlag) {
            onSizeChanged(_myXMLNode->getFloatValue("width",100), _myXMLNode->getFloatValue("height",100));
        }        
    }

    Window::~Window() {
    }
    
    void Window::onSizeChanged(int theWidth, int theHeight) {
        // take the first serios values, in case we are not setup explicit with spark values
        if (theWidth > 0 && theHeight > 0 && _myWidth == 0 && _myHeight == 0) {
            _myWidth = theWidth; 
            _myHeight = theHeight;
        }
    }

    void Window::render() const {        
        // get all views
        VectorOfComponentPtr myViews = getChildrenByType("View");
        for (std::vector<ComponentPtr>::const_iterator it = myViews.begin(); it != myViews.end(); ++it) {
            ViewPtr myView = boost::static_pointer_cast<spark::View>(*it);
            myView->activate(_myWidth, _myHeight);
            // find world and render it
            myView->renderWorld(getChildByName(myView->getWorldName()));
        }        
    }
}
