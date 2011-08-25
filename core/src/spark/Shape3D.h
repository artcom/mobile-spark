#ifndef _included_mobile_spark_Shape3D_
#define _included_mobile_spark_Shape3D_

#include "ShapeWidget.h"

using namespace mar;

namespace spark {
    ComponentPtr createShape3D(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
    class Shape3D : public ShapeWidget {
    public: 
        Shape3D(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~Shape3D();

    private:
        std::string _mySrc;
    };

    typedef boost::shared_ptr<Shape3D> Shape3DPtr;
};
#endif 
