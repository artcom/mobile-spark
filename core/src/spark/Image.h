#ifndef _included_mobile_spark_Image_
#define _included_mobile_spark_Image_

#include "ShapeWidget.h"

using namespace mar;

namespace spark {
    class Image : public ShapeWidget {
    public: 
        Image(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~Image();
        void onResume();
        void setSrc(std::string theSrc) {         AC_PRINT << " ######## Image.h";
_mySrc = theSrc; build();};

    private:
        void build();
        std::string _mySrc;
    };

    typedef boost::shared_ptr<Image> ImagePtr;
};
#endif 
