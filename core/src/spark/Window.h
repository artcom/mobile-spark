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

        void onTouch(EventPtr theEvent);
        virtual void render() const;
        virtual void onSizeChanged(int theWidth, int theHeight);
        vector4 getClearColor() const { return _myClearColor;};
    private:
        int _myWidth;
        int _myHeight;
        bool _myFullScreenFlag;
        vector4 _myClearColor;

        //picking -> move to other compilation unit?
        ComponentPtr pick2DBoxStyle(const unsigned int x, const unsigned int y) ;
        void runThroughTreeAndCollectPickedComponents(std::vector<std::pair<ComponentPtr, float> > & theList);
    };

    typedef boost::shared_ptr<Window> WindowPtr;

    //picking
    bool sortByZ(std::pair<ComponentPtr, float> i, std::pair<ComponentPtr, float> j);  
};

#endif
