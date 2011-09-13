#ifndef _included_mobile_spark_Rectangle_
#define _included_mobile_spark_Rectangle_

#include "ShapeWidget.h"

using namespace mar;

namespace spark {
    class Rectangle : public ShapeWidget {
    public: 
        Rectangle(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~Rectangle();
    
    private:
        vector4 _myColor;
    };

    typedef boost::shared_ptr<Rectangle> RectanglePtr;
};
#endif 
