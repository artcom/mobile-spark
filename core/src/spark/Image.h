#ifndef _included_mobile_spark_Image_
#define _included_mobile_spark_Image_

#include "I18nShapeWidget.h"

namespace spark {
    class Image : public I18nShapeWidget {
    public:
        Image(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Image();
        const std::string & getSrc() const {return data_;};
        void setSrc(const std::string & theSrc);
        void fitToSize(const float theWidth, const float theHeight);
        const vector2 & getTextureSize();

        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Image::SPARK_TYPE;};
    protected:
        virtual void build();
    private:
        vector2 _myTextureSize;
    };

    typedef masl::Ptr<Image> ImagePtr;
    typedef MemberFunctionEventCallback<Image, ImagePtr> ImageCB;
};
#endif
