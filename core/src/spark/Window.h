#ifndef _included_mobile_spark_Window_
#define _included_mobile_spark_Window_

#include <masl/XMLNode.h>
#include "Container.h"
#include "Event.h"
#include <mar/Canvas.h>


namespace spark {
    
    class Window : public Container {
    public: 
        Window(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
        virtual ~Window();

        void onTouch(EventPtr theEvent);
        virtual void render() const;
        virtual void onSizeChanged(EventPtr theEvent);
        virtual void onResume();        
        vector4 getClearColor() const { return _myClearColor;};
        vector2 getSize() const { return vector2(_myWidth, _myHeight);}
    private:
        int _myWidth;
        int _myHeight;
        bool _myFullScreenFlag;
        vector4 _myClearColor;
        mar::CanvasPtr _myGLCanvas;      

        //picking -> move to other compilation unit?
        ComponentPtr pick2DAABBStyle(const unsigned int x, const unsigned int y) ;
    };

    typedef boost::shared_ptr<Window> WindowPtr;
    typedef MemberFunctionEventCallback<Window, WindowPtr> WindowCB;

    //picking
    bool sortByZ(std::pair<ComponentPtr, float> i, std::pair<ComponentPtr, float> j);  
};

#endif
