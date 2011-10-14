#ifndef _included_mobile_spark_Window_
#define _included_mobile_spark_Window_

#include "Widget.h"
#include "Event.h"

namespace masl {
    class XMLNode;
    typedef masl::Ptr<XMLNode> XMLNodePtr;
};

namespace mar {
    class Canvas;
    typedef masl::Ptr<Canvas> CanvasPtr;
};

namespace spark {

    struct Orientation {
        static const char * const PORTRAIT;
        static const char * const LANDSCAPE;
        static const char * const FLOATING;
    };
    class Window : public Widget {
    public:
        Window(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Window();

        void onTouch(EventPtr theEvent);
        void render();
        virtual void onSizeChanged(EventPtr theEvent);
        virtual void onResume();
        virtual void realize();        
        std::string getOrientation() const { return _myOrientation;};
        vector4 getClearColor() const { return _myClearColor;};
        vector2 getSize() const;
        void setSize(int theWidth, int theHeight) {_myWidth=theWidth; _myHeight=theHeight;};

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
        std::vector<ComponentPtr> _myUnrealizedWorlds;
    };

    typedef masl::Ptr<Window> WindowPtr;
    typedef masl::WeakPtr<Window> WindowWeakPtr;
    typedef MemberFunctionEventCallback<Window, WindowWeakPtr> WindowCB;

    //picking
    bool sortByZ(std::pair<ComponentPtr, float> i, std::pair<ComponentPtr, float> j);
};

#endif
