#ifndef _included_mobile_spark_NinePatch_
#define _included_mobile_spark_NinePatch_

#include "Image.h"

namespace spark {
    class NinePatch : public Image {
    public:
        NinePatch(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~NinePatch();
        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return NinePatch::SPARK_TYPE;};
    protected:
        virtual mar::ShapePtr createCustomShape(const mar::MaterialPtr theMaterial);
    private:
        float edgeLeft_;
        float edgeTop_;
        float edgeRight_;
        float edgeBottom_;
    };

    typedef masl::Ptr<NinePatch> NinePatchPtr;
    typedef MemberFunctionEventCallback<NinePatch, NinePatchPtr> NinePatchCB;
};
#endif
