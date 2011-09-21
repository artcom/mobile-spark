#ifndef _included_mobile_spark_NinePatch_
#define _included_mobile_spark_NinePatch_

#include "ShapeWidget.h"
#include "I18nHandler.h"

using namespace mar;

namespace spark {
    class NinePatch : public ShapeWidget {
    public:
        NinePatch(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~NinePatch();
        virtual void onResume();
        virtual void realize();
        virtual void prerender(MatrixStack & theCurrentMatrixStack);
        void setSrc(std::string theSrc) { i18nHandler_->data_ = theSrc; _myDirtyFlag = true;};

        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return NinePatch::SPARK_TYPE;};
    private:
        void build();
        void attachToI18nItem();
        void handleI18nOnLanguageSwitch(const EventPtr theEvent = EventPtr());

        I18nHandlerPtr i18nHandler_;
        float edgeLeft_;
        float edgeTop_;
        float edgeRight_;
        float edgeBottom_;
    };

    typedef boost::shared_ptr<NinePatch> NinePatchPtr;
    typedef MemberFunctionEventCallback<NinePatch, NinePatchPtr> NinePatchCB;
};
#endif
