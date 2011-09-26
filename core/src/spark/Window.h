#ifndef _included_mobile_spark_Window_
#define _included_mobile_spark_Window_

#include "Widget.h"
#include "Event.h"

namespace masl {
    class XMLNode;
    typedef boost::shared_ptr<XMLNode> XMLNodePtr;
};

namespace mar {
    class Canvas;
    typedef boost::shared_ptr<Canvas> CanvasPtr;
};

namespace spark {

    class Window : public Widget {
    public:
        Window(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Window();

        void onTouch(EventPtr theEvent);
        virtual void render() const;
        virtual void onSizeChanged(EventPtr theEvent);
        virtual void onResume();
        virtual void realize();        
        vector4 getClearColor() const { return _myClearColor;};
        vector2 getSize() const;
        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Window::SPARK_TYPE;};
    private:
        int _myWidth;
        int _myHeight;
        bool _myFullScreenFlag;
        vector4 _myClearColor;
        mar::CanvasPtr _myGLCanvas;
        std::string _myOrientation;
        //picking -> move to other compilation unit?
        ComponentPtr pick2DAABBStyle(const unsigned int x, const unsigned int y) ;
    };

    typedef boost::shared_ptr<Window> WindowPtr;
    typedef MemberFunctionEventCallback<Window, WindowPtr> WindowCB;

    //picking
    bool sortByZ(std::pair<ComponentPtr, float> i, std::pair<ComponentPtr, float> j);
};

#endif
