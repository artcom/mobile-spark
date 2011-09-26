#ifndef _included_mobile_spark_Rectangle_
#define _included_mobile_spark_Rectangle_

#include "ShapeWidget.h"

namespace spark {
    class Rectangle : public ShapeWidget {
    public:
        Rectangle(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Rectangle();
        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Rectangle::SPARK_TYPE;};
    private:
        vector3 _myColor;
    };

    typedef boost::shared_ptr<Rectangle> RectanglePtr;
};
#endif
