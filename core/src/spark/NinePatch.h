#ifndef _included_mobile_spark_NinePatch_
#define _included_mobile_spark_NinePatch_

#include "ShapeWidget.h"

using namespace mar;

namespace spark {
    class NinePatch : public ShapeWidget {
    public: 
        NinePatch(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~NinePatch();
        virtual void onResume();
        static const char * const SPARK_TYPE;             
        virtual const char * const & getType() const { return NinePatch::SPARK_TYPE;};
    private:
        void build();
        std::string src_;
        float edgeLeft_;
        float edgeTop_;
        float edgeRight_;
        float edgeBottom_;
    };

    typedef boost::shared_ptr<NinePatch> NinePatchPtr;
};
#endif 
