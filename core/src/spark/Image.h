#ifndef _included_mobile_spark_Image_
#define _included_mobile_spark_Image_

#include "ShapeWidget.h"
#include "I18nHandler.h"

using namespace mar;

namespace spark {
    class Image : public ShapeWidget {
    public:
        Image(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~Image();
        virtual void onResume();
        virtual void realize();
        void setSrc(std::string theSrc) { i18nHandler_->data_ = theSrc; _myDirtyFlag = true;};

        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Image::SPARK_TYPE;};
    protected:
        virtual void build();
    private:
        I18nHandlerPtr i18nHandler_;
    };

    typedef boost::shared_ptr<Image> ImagePtr;
    typedef MemberFunctionEventCallback<Image, ImagePtr> ImageCB;
};
#endif
