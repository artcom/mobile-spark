#ifndef _included_mobile_spark_Window_
#define _included_mobile_spark_Window_

#include <masl/XMLNode.h>
#include "Container.h"


namespace spark {
    
    ComponentPtr createWindow(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
    
    class Window : public Container {
    public: 
        Window(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
        virtual ~Window();

        virtual void render() const;
        virtual void onSizeChanged(int theWidth, int theHeight);
        vector4 getClearColor() { return _myClearColor;}
    private:
        int _myWidth;
        int _myHeight;
        bool _myFullScreenFlag;
        vector4 _myClearColor;
    };

    typedef boost::shared_ptr<Window> WindowPtr;
};

#endif
