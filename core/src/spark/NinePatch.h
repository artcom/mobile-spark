#ifndef _included_mobile_spark_NinePatch_
#define _included_mobile_spark_NinePatch_

#include "I18nShapeWidget.h"

namespace spark {
    class NinePatch : public I18nShapeWidget {
    public:
        NinePatch(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~NinePatch();
        virtual void onResume();
        virtual void realize();
        void setSrc(const std::string & theSrc) { data_ = theSrc; _myDirtyFlag = true;};

        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return NinePatch::SPARK_TYPE;};
    protected:
        virtual void build();
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
