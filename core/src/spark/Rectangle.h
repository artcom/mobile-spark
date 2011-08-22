#ifndef _included_mobile_spark_Rectangle_
#define _included_mobile_spark_Rectangle_

#include <mar/Geometry.h>
#include "Widget.h"

using namespace mar;

namespace spark {
    ComponentPtr createRectangle(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
    class Rectangle : public Widget {
    public: 
        Rectangle(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~Rectangle();

    protected:
        virtual void renderShape() const;

    private:
        ShapePtr _myShape;
        std::vector<float> _myColor;
    };

    typedef boost::shared_ptr<Rectangle> RectanglePtr;
};
#endif 
