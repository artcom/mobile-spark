#ifndef _included_mobile_spark_Image_
#define _included_mobile_spark_Image_

#include "ShapeWidget.h"

using namespace mar;

namespace spark {
    ComponentPtr createImage(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
    class Image : public ShapeWidget {
    public: 
        Image(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~Image();

    private:
        std::string _mySrc;
    };

    typedef boost::shared_ptr<Image> ImagePtr;
};
#endif 
