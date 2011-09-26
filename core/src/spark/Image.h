#ifndef _included_mobile_spark_Image_
#define _included_mobile_spark_Image_

#include "ShapeWidget.h"
#include "Event.h"
#include "I18nHandler.h"

namespace spark {
    class Image : public ShapeWidget {
    public:
        Image(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Image();
        virtual void onResume();
        virtual void realize();
        void setSrc(std::string theSrc) { i18nHandler_->data_ = theSrc; _myDirtyFlag = true;};
        const vector2 & getTextureSize();

        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Image::SPARK_TYPE;};
    protected:
        virtual void build();
    private:
        vector2 _myTextureSize;        
        I18nHandlerPtr i18nHandler_;
    };

    typedef boost::shared_ptr<Image> ImagePtr;
    typedef MemberFunctionEventCallback<Image, ImagePtr> ImageCB;
};
#endif
