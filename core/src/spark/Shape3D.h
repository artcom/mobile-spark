#ifndef _included_mobile_spark_Shape3D_
#define _included_mobile_spark_Shape3D_

#include "ShapeWidget.h"

namespace spark {
    class Shape3D : public ShapeWidget {
    public:
        Shape3D(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Shape3D();
        void onResume();
        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Shape3D::SPARK_TYPE;};
    protected:
        virtual void build();
    private:
        std::string _mySrc;
    };

    typedef masl::Ptr<Shape3D> Shape3DPtr;
};
#endif
