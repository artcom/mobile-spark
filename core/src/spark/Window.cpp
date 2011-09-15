#include "Window.h"

#include <algorithm>
#include <masl/Logger.h>

#include "SparkComponentFactory.h"
#include "View.h"
#include "Visitors.h"

using namespace mar;

namespace spark {
    Window::Window(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, 
                   ComponentPtr theParent):
        Container(theApp, theXMLNode, theParent), 
        _myWidth(0), _myHeight(0){

        _myGLCanvas = CanvasPtr( new Canvas());
        _myGLCanvas->initGLState();
            
        _myFullScreenFlag = _myXMLNode->getAttributeAs<bool>("fullscreen", false);
        _myClearColor = _myXMLNode->getAttributeAs<vector4>("clearColor", vector4(1,1,1,1));

        // if we are running fullscreen, wait for the first onSize to setup viewport, otherwise use spark values
        //if (!_myFullScreenFlag) {
            onSizeChanged(_myXMLNode->getAttributeAs<float>("width",100), _myXMLNode->getAttributeAs<float>("height",100));
        //}        
    }

    Window::~Window() {
    }
    
    void 
    Window::onTouch(EventPtr theEvent) { 
        TouchEventPtr myEvent = boost::static_pointer_cast<TouchEvent>(theEvent);
        AC_INFO<<"hallo evt: "<< myEvent->getType() << " x: "<< myEvent->getX();
        ComponentPtr myPicked = pick2DAABBStyle(myEvent->getX(), myEvent->getY());
        if (myPicked) {
            EventPtr myEvent = EventPtr(new TouchEvent(TouchEvent::PICKED, myPicked));
            (*myEvent)();
            AC_PRINT << "______________________________________picked " << myPicked->getName();
        } else {
            AC_TRACE << "nothing picked";
        }
    }
    
    void Window::onResume() {
        AC_PRINT << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111";
        _myGLCanvas->initGLState();
    }

    void 
    Window::onSizeChanged(int theWidth, int theHeight) {
        _myWidth = theWidth; 
        _myHeight = theHeight;
    }

    void 
    Window::render() const {        
        _myGLCanvas->preRender(getClearColor());        
        // get all views
        VectorOfComponentPtr myViews = getChildrenByType("View");
        for (std::vector<ComponentPtr>::const_iterator it = myViews.begin(); it != myViews.end(); ++it) {
            ViewPtr myView = boost::static_pointer_cast<spark::View>(*it);
            if (myView->isVisible()) {
                AC_DEBUG << "render with : " << _myWidth << "/" << _myHeight;
                myView->activate(_myWidth, _myHeight);
                // find world and render it
                myView->renderWorld(getChildByName(myView->getWorldName()));
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
            visitComponents(myVisitor, getChildByName(myView->getWorldName()));
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
