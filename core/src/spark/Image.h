#ifndef _included_mobile_spark_Image_
#define _included_mobile_spark_Image_

#include <mar/Geometry.h>
#include "Widget.h"

using namespace mar;

namespace spark {
    ComponentPtr createImage(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
    class Image : public Widget {
    public: 
        Image(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~Image();

    protected:
        virtual void renderShape() const;

    private:
        ShapePtr _myShape;
        std::string _mySrc;
        void setVertexData(ElementPtr theElement, const float theWidth, const float theHeight);
    };

    typedef boost::shared_ptr<Image> ImagePtr;
};
#endif 
